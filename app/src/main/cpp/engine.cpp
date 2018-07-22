#include "engine.h"

#include <android/input.h>
#include <android/configuration.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <cerrno>
#include "log.h"
#include "glcontext.h"
#include <GLES2/gl2.h>
#include <unistd.h>
#include <string.h>
#include "game_loop.h"
#include "screen.h"
#include <chrono>
#include <cstring>
#include <errno.h>

inline static void spin_lock(volatile uint8_t* lock) {
	while (!__sync_bool_compare_and_swap(lock, 0, 1));
}

inline static void spin_unlock(volatile uint8_t* lock) {
	//__sync_bool_compare_and_swap(lock, *lock, 0);
	*lock = 0;
}

inline static void cond_wait(sem_t* cond, volatile uint8_t* lock) {
	spin_unlock(lock);
	sem_wait(cond);
	spin_lock(lock);
}

inline static void cond_broadcast(sem_t* cond) {
	sem_post(cond);
}

Engine* Engine::Create(jobject activity, JavaVM* javaVM, AAssetManager* assetManager) {
	int pipefd[2];

	if (pipe(pipefd) != 0) {
		LOGI("could not create pipe: %s", strerror(errno));
		return nullptr;
	}

	Engine* _this = new Engine;
	memset(_this, 0, sizeof(Engine));

	_this->java_vm = javaVM;
	_this->asset_manager = assetManager;

	JNIEnv* env;

	javaVM->GetEnv((void **)&env, JNI_VERSION_1_6);

	_this->activity = env->NewGlobalRef(activity);

	auto cls = env->GetObjectClass(_this->activity);
	_this->quitMethodID = env->GetMethodID(cls, "quit", "()V");

	env->DeleteLocalRef(cls);

	//javaVM->DetachCurrentThread();

	_this->readfd = pipefd[0];
	_this->writefd = pipefd[1];

	fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
	fcntl(pipefd[1], F_SETFL, O_NONBLOCK);

	sem_init(&_this->cond, 0, 1);

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&_this->thread, &attr, &Engine::StartMainLoop, _this);

	spin_lock(&_this->lock);
	while (!_this->running) {
		cond_wait(&_this->cond, &_this->lock);
	}
	spin_unlock(&_this->lock);

	return _this;
}

void* Engine::StartMainLoop(void* param) {
	Engine* _this = static_cast<Engine*>(param);

	_this->configuration = AConfiguration_new();
	AConfiguration_fromAssetManager(_this->configuration, _this->asset_manager);

	_this->PollCommandSource = Engine::ProcessCommand;
	_this->PollInputSource = Engine::ProcessInput;

	_this->looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	ALooper_addFd(_this->looper, _this->readfd, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, nullptr, &_this->PollCommandSource);

	spin_lock(&_this->lock);
	_this->running = true;
	cond_broadcast(&_this->cond);
	spin_unlock(&_this->lock);

	auto game_loop = new GameLoop;
	game_loop->engine = _this;
	game_loop->init();

	_this->game_loop = game_loop;

	auto last_time = std::chrono::high_resolution_clock::now();

	double freq = 1.0 / 30.0;

	PollSource* pollSource = nullptr;
	while (_this->running) {
		auto current_time = std::chrono::high_resolution_clock::now();
		double delta_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - last_time).count();

		while (ALooper_pollAll(_this->focused ? 0 : -1, nullptr, nullptr, reinterpret_cast<void **>(&pollSource)) >= 0) {
			if (pollSource != nullptr) {
				(*pollSource)(_this);

				if (!_this->running) break;
			}
		}

		if (delta_time >= freq) {
			if (_this->focused) {
				game_loop->tick(delta_time);

				if (GLContext::GetInstance()->Swap() != EGL_SUCCESS) {
					game_loop->unload();
					game_loop->load();
				}
			}
			last_time = current_time;
		}
	}
	game_loop->destroy();

	auto jvm = _this->java_vm;

	JNIEnv* env;

	jvm->AttachCurrentThread(&env, NULL);
	env->DeleteGlobalRef(_this->activity);
	jvm->DetachCurrentThread();

	GLContext::GetInstance()->Invalidate();
	//_this->TermDisplay();

	if (_this->input_queue != nullptr) {
		AInputQueue_detachLooper(_this->input_queue);
	}
	AConfiguration_delete(_this->configuration);

	ALooper_removeFd(_this->looper, _this->readfd);
	close(_this->readfd);
	close(_this->writefd);

	spin_lock(&_this->lock);
	_this->destroyed = true;
	cond_broadcast(&_this->cond);
	spin_unlock(&_this->lock);

	return nullptr;
}

void Engine::ProcessCommand(void* param) {
	Engine* _this = static_cast<Engine*>(param);

	int8_t state;
	if (read(_this->readfd, &state, sizeof(int8_t)) != sizeof(int8_t)) {
		LOGI("No data on command pipe!");
		return;
	}

	switch (state) {
		case RESUME:
			/*pthread_mutex_lock(&_this->mutex);
			_this->state = RESUME;
			pthread_cond_broadcast(&_this->cond);
			pthread_mutex_unlock(&_this->mutex);*/
			break;
		case PAUSE:
			/*pthread_mutex_lock(&_this->mutex);
			_this->state = PAUSE;
			pthread_cond_broadcast(&_this->cond);
			pthread_mutex_unlock(&_this->mutex);*/
			break;
		case DESTROY:
			_this->running = false;
			break;
		case WINDOW_GAINED_FOCUS:
			_this->focused = true;
			break;
		case WINDOW_LOST_FOCUS:
			_this->focused = false;
			break;
//		case SURFACE_REDRAW:
//			_this->DrawFrame();
//			break;
		case SURFACE_CREATED:
			spin_lock(&_this->lock);
			_this->window = _this->pending_window;
			cond_broadcast(&_this->cond);
			spin_unlock(&_this->lock);

			_this->InitDisplay();
			break;
		case SURFACE_DESTROYED:
			cond_broadcast(&_this->cond);

			_this->TermDisplay();

			spin_lock(&_this->lock);
			_this->window = nullptr;
			cond_broadcast(&_this->cond);
			spin_unlock(&_this->lock);
			break;
		case INPUT_QUEUE_CREATED:
			spin_lock(&_this->lock);
			_this->input_queue = _this->pending_input_queue;
			AInputQueue_attachLooper(_this->input_queue, _this->looper, LOOPER_ID_INPUT, nullptr, &_this->PollInputSource);
			cond_broadcast(&_this->cond);
			spin_unlock(&_this->lock);
			break;
		case INPUT_QUEUE_DESTROYED:
			spin_lock(&_this->lock);
			AInputQueue_detachLooper(_this->input_queue);
			_this->input_queue = nullptr;
			cond_broadcast(&_this->cond);
			spin_unlock(&_this->lock);
			break;
		default:
			break;
	}
}

//void MainActivity::ProcessInput(void* param) {
//	MainActivity* _this = static_cast<MainActivity*>(param);
//
//	AInputEvent* event = nullptr;
//	if (AInputQueue_getEvent(_this->input_queue, &event) >= 0) {
//		LOGI("New input event: type =%d\n", AInputEvent_getType(event));
//		if (AInputQueue_preDispatchEvent(_this->input_queue, event)) {
//			return;
//		}
//		AInputQueue_finishEvent(_this->input_queue, event, _this->engine->Input(event));
//	} else {
//		LOGI("Failure reading next input event: %s\n", strerror(errno));
//	}
//}

void Engine::InitDisplay() {
	GLContext* glcontext = GLContext::GetInstance();

	if (!initialized) {
		glcontext->Init(window);
		game_loop->load();
		initialized = true;
	} else if(glcontext->GetWindow() != window) {
		//assert(glcontext->GetANativeWindow());
		game_loop->unload();
		glcontext->Invalidate();
		glcontext->Init(window);
		game_loop->load();
	} else if (glcontext->Resume(window) == EGL_SUCCESS) {
		game_loop->unload();
		game_loop->load();
	} else {
		//assert(0);
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, glcontext->GetScreenWidth(), glcontext->GetScreenHeight());
}

void Engine::TermDisplay() {
	GLContext::GetInstance()->Suspend();
}

void Engine::ProcessInput(void* param) {
	auto _this = static_cast<Engine*>(param);

	if (_this->input_queue) {
		AInputEvent* event = nullptr;
		if (AInputQueue_getEvent(_this->input_queue, &event) >= 0) {
			if (!AInputQueue_preDispatchEvent(_this->input_queue, event)) {
				AInputQueue_finishEvent(_this->input_queue, event, InputManager::ProcessInputEvent(event));
			}
		}
	}
}

extern "C" JNIEXPORT jlong JNICALL Java_com_maxim_pipyaki_MainActivity_NativeOnCreate(JNIEnv* env, jobject instance, jobject assets) {
	JavaVM* javaVM;
	env->GetJavaVM(&javaVM);

	return reinterpret_cast<jlong>(Engine::Create(instance, javaVM, AAssetManager_fromJava(env, assets)));
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeOnResume(JNIEnv *env, jobject instance, jlong handle) {
	Engine* _this = reinterpret_cast<Engine*>(handle);

	static int8_t state[] = { RESUME };

	spin_lock(&_this->lock);
	write(_this->writefd, state, sizeof(int8_t));
	/*while (_this->state != RESUME) {
		cond_wait(&_this->cond, &_this->lock);
	}*/
	spin_unlock(&_this->lock);
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeOnPause(JNIEnv *env, jobject instance, jlong handle) {
	Engine* _this = reinterpret_cast<Engine*>(handle);

	static int8_t state[] = { PAUSE };

	spin_lock(&_this->lock);
	write(_this->writefd, state, sizeof(int8_t));
	/*while (_this->state != PAUSE) {
		cond_wait(&_this->cond, &_this->mutex);
	}*/
	spin_unlock(&_this->lock);
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeOnDestroy(JNIEnv *env, jobject instance, jlong handle) {
	Engine* _this = reinterpret_cast<Engine*>(handle);

	static int8_t state[] = { DESTROY };
	spin_lock(&_this->lock);
	write(_this->writefd, state, sizeof(int8_t));
	while (!_this->destroyed) {
		cond_wait(&_this->cond, &_this->lock);
	}
	spin_unlock(&_this->lock);
	delete _this;
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeWindowsFocusChanged(JNIEnv *env, jobject instance, jlong handle, jboolean hasFocus) {
	Engine* _this = reinterpret_cast<Engine*>(handle);

	if (hasFocus) {
		static int8_t state[] = { WINDOW_GAINED_FOCUS };
		write(_this->writefd, state, sizeof(int8_t));
	} else {
		static int8_t state[] = { WINDOW_LOST_FOCUS };
		write(_this->writefd, state, sizeof(int8_t));
	}
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeSurfaceRedraw(JNIEnv *env, jobject instance, jlong handle) {
	Engine* _this = reinterpret_cast<Engine*>(handle);

	static int8_t state[] = { SURFACE_REDRAW };
	write(_this->writefd, state, sizeof(int8_t));
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeSurfaceCreated(JNIEnv *env, jobject instance, jlong handle, jobject surface) {
	ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
	Engine* _this = reinterpret_cast<Engine*>(handle);

	static int8_t state[] = { SURFACE_CREATED };

	spin_lock(&_this->lock);
	_this->pending_window = window;
	write(_this->writefd, state, sizeof(int8_t));
	while (_this->window != window) {
		cond_wait(&_this->cond, &_this->lock);
	}
	spin_unlock(&_this->lock);
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeSurfaceDestroyed(JNIEnv *env, jobject instance, jlong handle) {
	Engine* _this = reinterpret_cast<Engine*>(handle);

	static int8_t state[] = { SURFACE_DESTROYED };

	spin_lock(&_this->lock);
	_this->pending_window = nullptr;
	write(_this->writefd, state, sizeof(int8_t));
	while (_this->window != nullptr) {
		cond_wait(&_this->cond, &_this->lock);
	}
	spin_unlock(&_this->lock);
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeInputQueueCreated(JNIEnv *env, jobject instance, jlong handle, jobject queue) {
	AInputQueue* input_queue = reinterpret_cast<AInputQueue*>(env->GetLongField(queue, env->GetFieldID(env->GetObjectClass(queue), "mPtr", "J")));
	Engine* _this = reinterpret_cast<Engine*>(handle);

	static int8_t state[] = { INPUT_QUEUE_CREATED };

	spin_lock(&_this->lock);
	_this->pending_input_queue = input_queue;
	write(_this->writefd, state, sizeof(int8_t));
	while (_this->input_queue != input_queue) {
		cond_wait(&_this->cond, &_this->lock);
	}
	spin_unlock(&_this->lock);
}

extern "C" JNIEXPORT void JNICALL Java_com_maxim_pipyaki_MainActivity_NativeInputQueueDestroyed(JNIEnv *env, jobject instance, jlong handle) {
	/*MainActivity* _this = reinterpret_cast<MainActivity*>(handle);

	static int8_t state[] = { INPUT_QUEUE_DESTROYED };

	spin_lock(&_this->lock);
	_this->pendingInputQueue = nullptr;
	write(_this->writefd, state, sizeof(int8_t));
	while (_this->inputQueue != nullptr) {
		cond_wait(&_this->cond, &_this->lock);
	}
	spin_unlock(&_this->lock);*/
}

#pragma once
#include <pthread.h>
#include <semaphore.h>
#include <jni.h>
#include "input.h"
#include <vector>

class Render;
class AAssetManager;
class ALooper;
class AInputQueue;
class AConfiguration;
class ANativeWindow;
class AInputEvent;
class GameLoop;

enum : uint8_t {
	RESUME,
	PAUSE,
	DESTROY,
	WINDOW_GAINED_FOCUS,
	WINDOW_LOST_FOCUS,
	SURFACE_REDRAW,
	SURFACE_CREATED,
	SURFACE_DESTROYED,
	INPUT_QUEUE_CREATED,
	INPUT_QUEUE_DESTROYED,
};

enum : uint8_t {
	LOOPER_ID_MAIN,
	LOOPER_ID_INPUT
};

typedef void(*PollSource)(void*);

struct Engine {
	PollSource PollCommandSource;
    PollSource PollInputSource;

	int readfd;
	int writefd;

	bool focused;
	bool running;
	bool destroyed;
	bool initialized;

	sem_t cond;
	uint8_t lock;
	pthread_t thread;

	JavaVM* java_vm;
	AAssetManager* asset_manager;

	GameLoop* game_loop;
	ALooper* looper;

	AInputQueue* input_queue;
	AInputQueue* pending_input_queue;

	AConfiguration* configuration;

	ANativeWindow* window;
	ANativeWindow* pending_window;

	static Engine* Create(jobject activity, JavaVM*, AAssetManager*);
	static void* StartMainLoop(void *);

	static void ProcessCommand(void*);
	static void ProcessInput(void*);

	void InitDisplay();
	void TermDisplay();

	jobject activity;
	jmethodID quitMethodID;
};


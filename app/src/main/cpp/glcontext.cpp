#include "glcontext.h"
#include "log.h"

#define EGL_STATUS_LOG(str) LOGD("Success: %s (%s:%d)", str, __FUNCTION__, __LINE__)
#define EGL_ERROR_LOG(str) LOGE("Failure: %s, error = 0x%08x (%s:%d)", str, eglGetError(), __FUNCTION__, __LINE__)

GLContext::GLContext()
        : window(nullptr),
          display(EGL_NO_DISPLAY),
          surface(EGL_NO_SURFACE),
          context(EGL_NO_CONTEXT),
          config(0),
          screen_width(0),
          screen_height(0),
          egl_context_initialized(false),
          context_valid(false) {}

GLContext::~GLContext() { Terminate(); }

bool GLContext::Init(ANativeWindow* window) {
    if (egl_context_initialized) return true;

    GLContext::window = window;

    InitEGLSurface();
    InitEGLContext();

    egl_context_initialized = true;

    return true;
}

bool GLContext::InitEGLSurface() {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, 0, 0);

    static const EGLint attributes[] = {
            EGL_RENDERABLE_TYPE,
            EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE,
            EGL_WINDOW_BIT,
            EGL_BLUE_SIZE,
            8,
            EGL_GREEN_SIZE,
            8,
            EGL_RED_SIZE,
            8,
            EGL_DEPTH_SIZE,
            24,
            EGL_NONE
    };

    EGLint num_configs;
    eglChooseConfig(display, attributes, &config, 1, &num_configs);

    if (!num_configs) {
        static const EGLint _attributes[] = {
                EGL_RENDERABLE_TYPE,
                EGL_OPENGL_ES2_BIT,
                EGL_SURFACE_TYPE,
                EGL_WINDOW_BIT,
                EGL_BLUE_SIZE,
                8,
                EGL_GREEN_SIZE,
                8,
                EGL_RED_SIZE,
                8,
                EGL_DEPTH_SIZE,
                16,
                EGL_NONE
        };

        eglChooseConfig(display, _attributes, &config, 1, &num_configs);
    }

    if (!num_configs) {
        LOGW("Unable to retrieve EGL config");
        return false;
    }

    surface = eglCreateWindowSurface(display, config, window, nullptr);
    eglQuerySurface(display, surface, EGL_WIDTH, &screen_width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &screen_height);

    return true;
}

bool GLContext::InitEGLContext() {
    static const EGLint attributes[] = {
            EGL_CONTEXT_CLIENT_VERSION,
            2,
            EGL_NONE
    };

    context = eglCreateContext(display, config, nullptr, attributes);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        EGL_ERROR_LOG("eglMakeCurrent");
        return false;
    }

    context_valid = true;
    return true;
}

EGLint GLContext::Swap() {
    if (!eglSwapBuffers(display, surface)) {
        switch (EGLint err = eglGetError()) {
            case EGL_BAD_SURFACE:
                InitEGLSurface();
                return EGL_SUCCESS;
            case EGL_CONTEXT_LOST:
            case EGL_BAD_CONTEXT:
                context_valid = false;
                Terminate();
                InitEGLContext();
            default:
                return err;
        }
    }
    return EGL_SUCCESS;
}

void GLContext::Terminate() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }

        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    window = nullptr;
    context_valid = false;
}

EGLint GLContext::Resume(ANativeWindow* window) {
    if (!egl_context_initialized) {
        Init(window);
        return EGL_SUCCESS;
    }

    GLContext::window = window;
    surface = eglCreateWindowSurface(display, config, window, nullptr);
    eglQuerySurface(display, surface, EGL_WIDTH, &screen_width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &screen_height);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_TRUE)
        return EGL_SUCCESS;

    EGL_ERROR_LOG("eglMakeCurrent");

    EGLint err = eglGetError();
    if (err == EGL_CONTEXT_LOST) {
        // Recreate context
        LOGI("Re-creating egl context");
        InitEGLContext();
    } else {
        // Recreate surface
        Terminate();
        InitEGLSurface();
        InitEGLContext();
    }

    return err;
}

void GLContext::Suspend() {
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(display, surface);
        surface = EGL_NO_SURFACE;
    }
}

bool GLContext::Invalidate() {
    Terminate();
    egl_context_initialized = false;
    return true;
}


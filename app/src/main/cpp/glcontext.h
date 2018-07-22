#pragma once
#include <EGL/egl.h>

class ANativeWindow;

class GLContext {
private:
    // EGL configurations
    ANativeWindow* window;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    EGLConfig config;

    // Screen parameters
    int32_t screen_width;
    int32_t screen_height;

    bool egl_context_initialized;
    bool context_valid;

    void Terminate();
    bool InitEGLSurface();
    bool InitEGLContext();

    GLContext(GLContext const&);
    void operator=(GLContext const&);
    GLContext();
    virtual ~GLContext();

public:
    static inline GLContext* GetInstance() {
        static GLContext instance;
        return &instance;
    }

    bool Init(ANativeWindow* window);
    EGLint Swap();
    bool Invalidate();

    void Suspend();
    EGLint Resume(ANativeWindow* window);

    inline ANativeWindow* GetWindow(void) const { return window; }
    inline int32_t GetScreenWidth() const { return screen_width; }
    inline int32_t GetScreenHeight() const { return screen_height; }
};

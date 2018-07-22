#pragma once

#include <cstdint>
#include "glmath.h"
#include "delegate.h"

struct AInputEvent;

enum class InputEventType {
    PointerDown,
    PointerUp,
    PointerMove
};

struct Pointer {
    friend struct InputManager;

    constexpr Pointer() {}

    inline int getId() const {
        return id;
    }

    inline ivec2 delta() const {
        return pos - prev;
    }

    inline ivec2 position() const {
        return pos;
    }

    inline ivec2 dragOffset() const {
        return pressed ? pos - start : ivec2();
    }

private:
    int id = -1;

    ivec2 prev;
    ivec2 start;
    ivec2 pos;

    bool pressed = 0;
};

struct InputListener {
    virtual void OnPointerDown(const Pointer& pointer) = 0;
    virtual void OnPointerMove(const Pointer& pointer) = 0;
    virtual void OnPointerUp(const Pointer& pointer) = 0;
};

struct InputManager {
    friend struct Engine;
private:
    InputManager() = default;

    static std::vector<InputListener*> listeners;

    static Pointer pointers[10];

    static bool ProcessTouchEvent(int action, int x, int y, int pid, int64_t time, bool isTouch);

    static void DispatchTouch(size_t idx, InputEventType type, Pointer& state, ivec2 pos, int64_t time, bool isTouch);

    static int ProcessInputEvent(AInputEvent *event);

public:
    static void AddListener(InputListener* && listener) {
        assert(listener);

        for (auto it = listeners.begin(); it != listeners.end(); it++) {
            if (*it == listener) return;
        }

        listeners.push_back(listener);
    }

    static void RemoveListener(InputListener* && listener) {
        assert(listener);

        for (auto it = listeners.begin(); it != listeners.end(); it++) {
            if (*it != listener) continue;

            listeners.erase(it);
            break;
        }
    }

    static size_t TouchCount();
    static Pointer GetTouch(size_t index);
};
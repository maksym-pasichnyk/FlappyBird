#include "input.h"
#include "screen.h"

#include <android/input.h>

std::vector<InputListener*> InputManager::listeners;
Pointer InputManager::pointers[10];

void InputManager::DispatchTouch(size_t idx, InputEventType type, Pointer& state, ivec2 pos, int64_t time, bool isTouch)
{
    switch (type) {
        case InputEventType::PointerDown:
            state.start = pos;
            state.pressed = true;
            break;
        case InputEventType::PointerUp:
            state.pressed = false;
            break;
        default: break;
    }
    state.prev = state.pos;
    state.pos = pos;

    void(InputListener::*func)(const Pointer&) = nullptr;

    switch (type) {
        case InputEventType::PointerDown:
            func = &InputListener::OnPointerDown;
            break;
        case InputEventType::PointerMove:
            func = &InputListener::OnPointerMove;
            break;
        case InputEventType::PointerUp: {
            func = &InputListener::OnPointerUp;
            break;
        }
    }

    for (auto& listener : listeners) {
        (listener->*func)(state);
    }
}

bool InputManager::ProcessTouchEvent(int action, int x, int y, int pid, int64_t time, bool isTouch)
{
    static constexpr size_t count = sizeof(pointers) / sizeof(Pointer);

    ivec2 pos = { x, Screen::height() - y };

    switch(action) {
        case AMOTION_EVENT_ACTION_DOWN:
        case AMOTION_EVENT_ACTION_POINTER_DOWN:
            for (size_t i = 0; i < count; i++) {
                auto &p = pointers[i];

                if (pointers[i].id == -1) {
                    p.id = pid;
                    DispatchTouch(i, InputEventType::PointerDown, p, pos, time, isTouch);
                    break;
                }
            }
            break;
        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_CANCEL:
            for (size_t i = 0; i < count; i++) {
                auto &p = pointers[i];

                if (p.pressed) {
                    DispatchTouch(i, InputEventType::PointerUp, p, p.pos, time, isTouch);
                    p.id = -1;
                }
            }
            break;
        case AMOTION_EVENT_ACTION_POINTER_UP:
            for (size_t i = 0; i < count; i++) {
                auto &p = pointers[i];

                if (p.id == pid) {
                    DispatchTouch(i, InputEventType::PointerUp, p, pos, time, isTouch);
                    p.id = -1;
                    break;
                }
            }
            break;
        default:
            for (size_t i = 0; i < count; i++) {
                auto &p = pointers[i];

                if (p.id == pid) {
                    DispatchTouch(i, InputEventType::PointerMove, p, pos, time, isTouch);
                    break;
                }
            }
            break;
    }

    return 1;
}

int InputManager::ProcessInputEvent(AInputEvent *event) {
    auto type = AInputEvent_getType(event);
    auto source = AInputEvent_getSource(event);
    switch (type) {
        case AINPUT_EVENT_TYPE_MOTION: {
            int eventAction = AMotionEvent_getAction(event);

            switch (source) {
                case AINPUT_SOURCE_TOUCHSCREEN: {
                    int action = eventAction & AMOTION_EVENT_ACTION_MASK;

                    if (action == AMOTION_EVENT_ACTION_UP || action == AMOTION_EVENT_ACTION_CANCEL) {
                        int x = int(AMotionEvent_getX(event, 0));
                        int y = int(AMotionEvent_getY(event, 0));

                        int32_t pid = AMotionEvent_getPointerId(event, 0);
                        int64_t time = AMotionEvent_getEventTime(event);

                        ProcessTouchEvent(action, x, y, pid, time, true);
                        return 1;
                    }

                    size_t count = AMotionEvent_getPointerCount(event);

                    int id = eventAction >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    for (size_t i = 0; i < count; i++) {
                        int _action = id == i ? action : AMOTION_EVENT_ACTION_MOVE;

                        int x = int(AMotionEvent_getX(event, 0));
                        int y = int(AMotionEvent_getY(event, 0));

                        int32_t pid = AMotionEvent_getPointerId(event, i);
                        int64_t time = AMotionEvent_getEventTime(event);

                        ProcessTouchEvent(_action, x, y, pid, time, true);
                    }
                    return 1;
                }
                default: break;
            }
        } break;
        default: break;
    }
    return 0;
}

size_t InputManager::TouchCount() {
    size_t count = 0;

    for (auto& p : pointers) {
        if (p.pressed) count++;
    }

    return count;
}

Pointer InputManager::GetTouch(size_t index) {
    for (auto& p : pointers) {
        if (p.pressed && (index-- == 0)) {
            return p;
        }
    }
}

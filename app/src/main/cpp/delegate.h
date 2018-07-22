#pragma once

#include <cassert>
#include <utility>
#include <vector>

template <typename ...Args>
struct delegate {
public:
    typedef void(*type)(Args...);

    constexpr delegate() = default;

    void operator +=(const type& listener) {
        assert(listener);

        for (auto it = listeners.begin(); it != listeners.end(); it++) {
            if (*it == listener) return;
        }

        listeners.push_back(listener);
    }

    void operator -=(const type& listener) {
        assert(listener);

        for (auto it = listeners.begin(); it != listeners.end(); it++) {
            if (*it != listener) continue;

            listeners.erase(it);
            break;
        }
    }

    void operator()(Args&&... args) {
        for (auto& listener : listeners) {
            listener(std::forward<Args>(args)...);
        }
    }
private:
    std::vector<type> listeners;
};
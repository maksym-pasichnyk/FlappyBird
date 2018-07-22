#pragma once

#include "component.h"
#include "scene.h"
#include "log.h"

/*void dump_vtable(void **vtable) {
    int index = 0;

    while (*vtable) {
        Dl_info info;
        dladdr(*vtable++, &info);
        LOGI("%d %p %s \n", index++, info.dli_saddr, info.dli_sname);
    }
}

void** find_vtable(const char *symbol) {
    return (void **)((int)dlsym(RTLD_DEFAULT, symbol) + 8);
}

static inline const void* adjust_pointer(const void* p, std::ptrdiff_t off) {
    return reinterpret_cast<const void *>(reinterpret_cast<const char *>(p) + off);
}

static inline const void* get_vtable(const void* p) {
    return *reinterpret_cast<void*const*>(p);
}

static inline const std::type_info* get_type_info(const void* p) {
    return *reinterpret_cast<std::type_info *const *>(adjust_pointer(get_vtable(p), -sizeof(void *)));
}*/

//template <typename To = void*, typename From>
//static inline To fast_dynamic_cast(From* from) {
//    auto type = get_type_info(from);
//    return nullptr;
//};

struct GameObject {
private:
    std::vector<Component*> components;

    GameObject(const GameObject&) = delete;

public:
    bool active = true;

    Transform2D* transform;

    GameObject();

    ~GameObject();

    template <typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    T* AddComponent() {
        T* component = new T;
        component->gameObject = this;
        components.push_back(component);
        return component;
    }

    template <typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    T* GetComponent() {
        for (auto component : components) {
            if (auto ptr = dynamic_cast<T*>(component)) {
                return ptr;
            }
        }

        return nullptr;
    }

    template <typename T, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    std::vector<T*> GetComponents() {
        std::vector<T*> result;
        for (auto component : components) {
            if (auto ptr = dynamic_cast<T*>(component)) {
                result.push_back((T*)component);
            }
        }

        return result;
    }
};


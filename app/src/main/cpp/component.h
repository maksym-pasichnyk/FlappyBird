#pragma once

#include <cstddef>

struct Transform2D;
struct GameObject;

struct Component {
protected:
    friend struct GameObject;

    Component() = default;
    virtual ~Component() = default;

public:
    bool enabled = true;

    GameObject* gameObject;

    Transform2D* transform();
};
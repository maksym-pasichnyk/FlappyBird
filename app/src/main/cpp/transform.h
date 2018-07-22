#pragma once

#include "glmath.h"
#include "component.h"

struct Transform2D : Component {
    vec2 position = {0, 0};
    vec2 size = {0, 0};

    float rotation = 0;

    mat4x4 matrix() {
        return  mat4x4::translate({position, 0}) * mat4x4::rotate(0, 0, rotation * float(M_PI) / 180.f);
    }
};
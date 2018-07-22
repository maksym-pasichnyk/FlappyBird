#pragma once

#include <vector>
#include <GLES2/gl2.h>
#include "glmath.h"
#include "color.h"
#include "material.h"
#include "transform.h"
#include "screen.h"

struct Camera {
    static Camera* main;

    Transform2D transform;
    float aspect;

    Color color;
    vec4 viewport;

    mat4x4 matrix();
    mat4x4 ui_matrix();
};
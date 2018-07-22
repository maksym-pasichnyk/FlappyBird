#pragma once

#include <GLES2/gl2.h>

struct Material {
    Material() = default;
    Material(const char* vertex, const char* fragment);

    void release();

    GLuint shader;
    GLuint VERTEX;
    GLuint TEXTURE;
    GLuint WORLD_MATRIX;
    GLuint UI_MATRIX;
    GLuint TRANSFORM_MATRIX;
};
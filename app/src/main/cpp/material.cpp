#include "material.h"
#include "resources.h"

Material::Material(const char *vertex, const char *fragment) {
    shader = ResourceManager::LoadShader(vertex, fragment);

    VERTEX = static_cast<GLuint>(glGetAttribLocation(shader, "VertexLocation"));
    TEXTURE = static_cast<GLuint>(glGetAttribLocation(shader, "TextureLocation"));
    WORLD_MATRIX = static_cast<GLuint>(glGetUniformLocation(shader, "WORLD_MATRIX"));
    UI_MATRIX = static_cast<GLuint>(glGetUniformLocation(shader, "UI_MATRIX"));
    TRANSFORM_MATRIX = static_cast<GLuint>(glGetUniformLocation(shader, "TRANSFORM_MATRIX"));
}

void Material::release() {
    glDeleteShader(shader);
    shader = 0;
    VERTEX = 0;
    TEXTURE = 0;
    WORLD_MATRIX = 0;
    UI_MATRIX = 0;
    TRANSFORM_MATRIX = 0;
}
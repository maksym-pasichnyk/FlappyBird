#include "camera.h"

Camera* Camera::main;

mat4x4 Camera::matrix() {
    auto projection = mat4x4::orthographic(138, aspect, -2, 2);
    return projection * transform.matrix();
}

mat4x4 Camera::ui_matrix() {
    return mat4x4::orthographic(Screen::height(), aspect, -2, 2);
}

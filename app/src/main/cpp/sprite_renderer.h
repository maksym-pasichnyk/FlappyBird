#pragma once

#include "renderer.h"
#include "sprite.h"
#include "transform.h"

struct SpriteRenderer : public Renderer {
    Sprite sprite;
    int clip;

    void render(const Material& material) override {
        glUniformMatrix4fv(material.TRANSFORM_MATRIX, 1, GL_FALSE, &transform()->matrix()[0][0]);

        sprite.setClip(clip);
        sprite.render(material);
    }
};
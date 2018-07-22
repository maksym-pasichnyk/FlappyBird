#pragma once

#include "material.h"
#include "texture.h"
#include "screen.h"

struct Graphics {
    static void DrawTexture(const Material& material, const Texture2D& texture, GLfloat vertices[12], GLfloat coordinates[12]) {
        glBindTexture(GL_TEXTURE_2D, texture.textureID);

        glEnableVertexAttribArray(material.VERTEX);
        glEnableVertexAttribArray(material.TEXTURE);

        glVertexAttribPointer(material.VERTEX, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(material.TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, coordinates);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(material.VERTEX);
        glDisableVertexAttribArray(material.TEXTURE);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    static void DrawTexture(const Material& material, const Texture2D& texture, float x, float y, float width, float height) {
        float left = x;
        float right = (x + width);
        float bottom = y;
        float top = (y + height);

        GLfloat vertices[12] = {
                left, bottom, right, bottom, right, top, right, top, left, top, left, bottom
        };
        GLfloat coordinates[12] = {
                0, 1,
                1, 1,
                1, 0,
                1, 0,
                0, 0,
                0, 1
        };

        DrawTexture(material, texture, vertices, coordinates);
    }

    static void DrawTexture(const Material& material, const Texture2D& texture, float xSrc, float ySrc, float widthSrc, float heightSrc, float x, float y, float width, float height) {
        float left = x;
        float right = (x + width);
        float bottom = y;
        float top = (y + height);

        GLfloat vertices[12] = {
            left, bottom, right, bottom, right, top, right, top, left, top, left, bottom
        };

        float TextureWidthScale = 1.0f / static_cast<float>(texture.width);
        float TextureHeightScale = 1.0f / static_cast<float>(texture.height);

        float rightSrc = xSrc * TextureWidthScale;
        float leftSrc = (xSrc + widthSrc) * TextureWidthScale;
        float topSrc = ySrc * TextureHeightScale;
        float bottomSrc = (ySrc + heightSrc) * TextureHeightScale;

        GLfloat coordinates[12] = {
            rightSrc, bottomSrc, leftSrc, bottomSrc, leftSrc, topSrc, leftSrc, topSrc, rightSrc, topSrc, rightSrc, bottomSrc
        };

        DrawTexture(material, texture, vertices, coordinates);
    }

    static void Draw9Path(const Material& material, const Ninepath& ninepath, float x, float y, float w, float h) {
        float _1 = h - ninepath.top + y;
        float _2 = ninepath.texture.width - ninepath.right;
        float _3 = ninepath.texture.height - ninepath.bottom;
        float _4 = ninepath.right - ninepath.left;
        float _5 = ninepath.bottom - ninepath.top;
        float _6 = x + ninepath.left;
        float _7 = w - ninepath.left - ninepath.right;
        float _8 = h - _3 - ninepath.top;
        float _9 = w - _2 - ninepath.left;
        float _10 = y + ninepath.top;
        float _11 = x + w - _2;

        DrawTexture(material, ninepath.texture, 0.0f, 0.0f, ninepath.left, ninepath.top, x, _1, ninepath.left, ninepath.top);
        DrawTexture(material, ninepath.texture, ninepath.left, 0.0f, _4, ninepath.top, _6, _1, _7, ninepath.top);
        DrawTexture(material, ninepath.texture, ninepath.right, 0.0f, _2, ninepath.top, _11, _1, _2, ninepath.top);

        DrawTexture(material, ninepath.texture, 0.0f, ninepath.top, ninepath.left, _5, x, _10, ninepath.left, _8);
        DrawTexture(material, ninepath.texture, ninepath.left, ninepath.top, _4, _5, _6, y + _3, _9, _8);
        DrawTexture(material, ninepath.texture, ninepath.right, ninepath.top, _2, _5, _11, _10, ninepath.left, _8);

        DrawTexture(material, ninepath.texture, 0.0f, ninepath.bottom, ninepath.left, _3, x, y, ninepath.left, _3);
        DrawTexture(material, ninepath.texture, ninepath.left, ninepath.bottom, _4, _3, _6, y, _7, _3);
        DrawTexture(material, ninepath.texture, ninepath.right, ninepath.bottom, _2, _3, _11, y, _2, _3);
    }

    static void Draw9Path(const Material& material, const Texture2D& texture, float left, float right, float bottom, float top, float x, float y, float w, float h) {
        float _1 = h - top + y;
        float _2 = texture.width - right;
        float _3 = texture.height - bottom;
        float _4 = right - left;
        float _5 = bottom - top;
        float _6 = x + left;
        float _7 = w - _2 - left;
        float _8 = h - _3 - top;
        float _10 = y + top;
        float _11 = x + w - _2;

        DrawTexture(material, texture, 0.0f, 0.0f, left, top, x, _1, left, top);
        DrawTexture(material, texture, left, 0.0f, _4, top, _6, _1, _7, top);
        DrawTexture(material, texture, right, 0.0f, _2, top, _11, _1, _2, top);

        DrawTexture(material, texture, 0.0f, top, left, _5, x, _10, left, _8);
        DrawTexture(material, texture, left, top, _4, _5, _6, y + _3, _7, _8);
        DrawTexture(material, texture, right, top, _2, _5, _11, _10, left, _8);

        DrawTexture(material, texture, 0.0f, bottom, left, _3, x, y, left, _3);
        DrawTexture(material, texture, left, bottom, _4, _3, _6, y, _7, _3);
        DrawTexture(material, texture, right, bottom, _2, _3, _11, y, _2, _3);
    }

    static void DrawTexture(const Material& material, const Texture2D &texture, Rect rect) {
        DrawTexture(material, texture, rect.x, rect.y, rect.w, rect.h);
    }

    static void DrawTexture(const Material& material, const Texture2D &texture) {
        float left = 0;
        float right = texture.width;
        float bottom = 0;
        float top = texture.height;

        GLfloat vertices[12] = {
                left, bottom, right, bottom, right, top, right, top, left, top, left, bottom
        };

        GLfloat coordinates[12] = {
                0, 0,
                1, 0,
                1, 1,
                1, 1,
                0, 1,
                0, 0
        };

        DrawTexture(material, texture, vertices, coordinates);
    }
};
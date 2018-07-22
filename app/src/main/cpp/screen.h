#pragma once

#include <GLES2/gl2.h>
#include "glmath.h"
#include "texture.h"
#include "ninepath.h"
#include "font.h"
#include "property.h"

struct Rect {
	float x, y, w, h;

	Rect() : x(0), y(0), w(0), h(0) {}

	Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

	bool contains(float _x, float _y) const {
		return (_x >= x) && (_x <= (x + w)) && (_y >= y) && (_y <= (y + h));
	}

	vec2 center() const {
		return {x + w * 0.5f, y + h * 0.5f};
	}
};

class GLContext;

class Screen {
public:
//	static GLContext* glcontext;

	static int32_t width();
	static int32_t height();
	//static int32_t height();
//	static void Load();
//	static void Unload();
//
//	static void SetMVPMatrix(const mat4x4&);
//	static void DrawTexture(const Texture2D&);
//	static void DrawTexture(const Texture2D&, float, float, float, float);
//	static void DrawTexture(const Texture2D&, float, float, float, float, float, float, float, float);
//
//	static void DrawTexture(const Texture2D&, GLfloat[12], GLfloat[12]);
//
//	static void Draw9Path(const Ninepath&, float, float, float, float);
//	static void Draw9Path(const Texture2D&, float, float, float, float, float, float, float, float);
//
//	static void DrawText(const char *text, float size, float x, float y);
//
//	static float GetTextWidth(const char *string, int size);
//
//	static void DrawTexture(const Texture2D &texture, Rect rect);
};

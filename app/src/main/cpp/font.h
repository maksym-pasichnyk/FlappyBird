#pragma once

#include <string>
#include "texture.h"
#include "glmath.h"

struct Material;

struct Font {
	Texture2D texture;
	vec4 glyphs[256];

	static Font Create(const char*, const char*, const char*);

	void drawText(const Material&, const char *text, float size, float x, float y);

	float getTextWidth(const char *string, int size);
};
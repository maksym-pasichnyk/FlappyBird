#pragma once

#include "screen.h"
#include "resources.h"

struct Sprite {
	Texture2D texture;
	vec4 border;
	vec2 pivot;
	Rect rect;
	int clip;

	static Sprite Create(const char* path, Rect rect, vec2 pivot = {0.5f, 0.5f}, vec4 border = {0, 0, 0, 0});

	static Sprite Create(const Texture2D &texture, Rect rect, vec2 pivot = {0.5f, 0.5f}, vec4 border = {0, 0, 0, 0});

	void setClip(int clip);

	void render(const Material &position);
};
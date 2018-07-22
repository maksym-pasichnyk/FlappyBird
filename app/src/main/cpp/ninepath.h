#pragma once

#include "texture.h"

struct Ninepath {
	Texture2D texture;
	float left, right, bottom, top;

	Ninepath() {}
	Ninepath(Texture2D, float, float, float, float);
};
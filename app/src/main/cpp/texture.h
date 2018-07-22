#pragma once

#include <GLES2/gl2.h>
#include <vector>

class Texture2D {
public:
    GLuint textureID;
	uint32_t width;
	uint32_t height;

	//std::vector<uint8_t> pixels;

public:
	Texture2D();

    static Texture2D Empty;
    static Texture2D Create(uint8_t*, uint32_t width, uint32_t height);

    void release();
};

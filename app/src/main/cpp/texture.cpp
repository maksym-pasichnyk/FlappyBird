#include <cstdio>
#include "texture.h"
#include "resources.h"

Texture2D Texture2D::Empty;

Texture2D::Texture2D() : textureID(0), width(0), height(0) {}

Texture2D Texture2D::Create(uint8_t* pixels, uint32_t width, uint32_t height) {
	Texture2D _this;
	_this.width = width;
	_this.height = height;

	glGenTextures(1, &_this.textureID);
	glBindTexture(GL_TEXTURE_2D, _this.textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	return _this;
}

void Texture2D::release() {
    glDeleteTextures(1, &textureID);
}

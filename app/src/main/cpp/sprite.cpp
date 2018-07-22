#include "sprite.h"
#include "graphics.h"

Sprite Sprite::Create(const char *path, Rect rect, vec2 pivot, vec4 border) {
	return Create(ResourceManager::Load<Texture2D>(path), rect, pivot, border);
}

Sprite Sprite::Create(const Texture2D &texture, Rect rect, vec2 pivot, vec4 border) {
	return { texture, border, pivot, rect, 0};
}

void Sprite::setClip(int clip) {
	Sprite::clip = clip;
}

void Sprite::render(const Material& material) {
    Graphics::DrawTexture(material, texture, rect.w * clip, rect.y, rect.w, rect.h, -rect.w * pivot.x, -rect.h * pivot.y, rect.w, rect.h);
}

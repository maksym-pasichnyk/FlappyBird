#include "font.h"
#include "resources.h"
#include "buffer.h"
#include "material.h"

Font Font::Create(const char* png, const char* cfg, const char* txt) {
	assert(png && cfg && txt);

	Font _this;
	_this.texture = ResourceManager::Load<Texture2D>(png);
	BufferStream config = ResourceManager::ReadFile(cfg);
	BufferStream chars = ResourceManager::ReadFile(txt);

	vec4 glyph;
	glyph.x = 0.0f;
	glyph.y = 0.0f;
	glyph.z = 0.0f;
	glyph.w = _this.texture.height;

	while (chars) {
		auto ch = chars.read<uint8_t>();

		if (!isprint(ch)) continue;

		glyph.z = config.read<uint8_t>();
		_this.glyphs[ch] = glyph;
		glyph.x += glyph.z;
	}

	return _this;
}

void Font::drawText(const Material& material, const char *text, float size, float x, float y) {
	if (!text) return;

	float s = size / static_cast<float>(texture.height);

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> coordinates;

	float TextureWidthScale = 1.0f / static_cast<float>(texture.width);
	float TextureHeightScale = 1.0f / static_cast<float>(texture.height);

	while (*text) {
		if (isspace(*text)) {
			x += size;
			text++;
			continue;
		}

		const vec4& glyph = glyphs[*text++];

		float xSrc = glyph.x;
		float ySrc = glyph.y;
		float widthSrc = glyph.z;
		float heightSrc = glyph.w;
		float width = glyph.z * s;
		float height = glyph.w * s;

		float left = x;
		float right = (x + width);
		float bottom = y;
		float top = (y + height);

		vertices.insert(vertices.end(), {
				left, bottom, right, bottom, right, top, right, top, left, top, left, bottom
		});

		float rightSrc = xSrc * TextureWidthScale;
		float leftSrc = (xSrc + widthSrc) * TextureWidthScale;
		float topSrc = ySrc * TextureHeightScale;
		float bottomSrc = (ySrc + heightSrc) * TextureHeightScale;

		coordinates.insert(coordinates.end(), {
				rightSrc, bottomSrc, leftSrc, bottomSrc, leftSrc, topSrc, leftSrc, topSrc, rightSrc, topSrc, rightSrc, bottomSrc
		});

		x += glyph.z * s;
	}

	glBindTexture(GL_TEXTURE_2D, texture.textureID);

	glEnableVertexAttribArray(material.VERTEX);
	glEnableVertexAttribArray(material.TEXTURE);

	glVertexAttribPointer(material.VERTEX, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
	glVertexAttribPointer(material.TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, &coordinates[0]);

	glDrawArrays(GL_TRIANGLES, 0, int(vertices.size() / 2));

	glDisableVertexAttribArray(material.VERTEX);
	glDisableVertexAttribArray(material.TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

float Font::getTextWidth(const char *string, int size) {
	float s = size / (float) texture.height;
	float w = 0;

	while (*string) {
		if (isspace(*string)) {
			w += size;
		} else {
			w += glyphs[*string].z * s;
		}
		string++;
	}
	return w;
}
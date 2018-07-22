#pragma once

#include <vector>
#include <GLES2/gl2.h>
#include "texture.h"

class AAssetManager;

class ResourceManager {
private:
    static AAssetManager* AssetManager;

public:
    static void Init(AAssetManager*);
    static std::vector<uint8_t> ReadFile(const char*);
    static GLuint LoadShader(const char*, const char*);

	template <typename T> static T Load(const char *path);
};
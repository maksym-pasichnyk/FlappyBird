#include <fstream>
#include <android/asset_manager.h>
#include "resources.h"
#include "shader.h"
#include <android/bitmap.h>
#include <malloc.h>
#include "texture.h"
#include "lodepng.h"
#include "tile.h"
#include "buffer.h"

uint16_t endian_swap_16(uint16_t value) {
	return (value >> 8) | (value << 8);
}

uint32_t endian_swap_32(uint32_t value) {
	return (value >> 24) | ((value << 8) & 0x00FF0000) | ((value >> 8) & 0x0000FF00) | (value << 24);
}

AAssetManager* ResourceManager::AssetManager = nullptr;

void ResourceManager::Init(AAssetManager* AssetManager) {
	ResourceManager::AssetManager = AssetManager;
}

std::vector<uint8_t> ResourceManager::ReadFile(const char* fileName) {
	std::ifstream file(fileName, std::ios::binary);
	if (file) {
		//LOGI("reading:%s", s.c_str());
		file.seekg(0, std::ifstream::end);
		size_t fileSize = static_cast<size_t>(file.tellg());
		file.seekg(0, std::ifstream::beg);

		std::vector<uint8_t> buffer;
		buffer.reserve(fileSize);
		buffer.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
		return buffer;
	} else {
		AAsset* assetFile = AAssetManager_open(AssetManager, fileName, AASSET_MODE_BUFFER);
		if (!assetFile) {
			return {};
		}
		uint8_t* data = (uint8_t *) AAsset_getBuffer(assetFile);
		if (data == nullptr) {
			AAsset_close(assetFile);

			//LOGI("Failed to load:%s", fileName);
			return {};
		}

		size_t size = static_cast<size_t>(AAsset_getLength(assetFile));

		std::vector<uint8_t> buffer;
		buffer.reserve(size);
		buffer.assign(data, data + size);

		AAsset_close(assetFile);
		return buffer;
	}
}

GLuint ResourceManager::LoadShader(const char* vertexFile, const char* fragmentFile) {
	GLuint vertexShader = 0, fragmentShader = 0;

	if (!ShaderManager::CompileShader(&vertexShader, GL_VERTEX_SHADER, vertexFile)) {
		//LOGI("Failed to compile vertex shader");
		return 0;
	}

	if (!ShaderManager::CompileShader(&fragmentShader, GL_FRAGMENT_SHADER, fragmentFile)) {
		//LOGI("Failed to compile fragment shader");
		return 0;
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	if (!ShaderManager::LinkProgram(program)) {
		if (vertexShader) {
			glDeleteShader(vertexShader);
			vertexShader = 0;
		}
		if (fragmentShader) {
			glDeleteShader(fragmentShader);
			fragmentShader = 0;
		}
		if (program) {
			glDeleteProgram(program);
		}

		return 0;
	}

	if (vertexShader) glDeleteShader(vertexShader);
	if (fragmentShader) glDeleteShader(fragmentShader);

	if (!ShaderManager::ValidateProgram(program)) {
		if (program) {
			glDeleteProgram(program);
		}
		return 0;
	}

	return program;
}

template<>
Texture2D ResourceManager::Load<Texture2D>(const char* path) {
	auto data = ResourceManager::ReadFile(path);

	if (data.empty()) {
		return Texture2D::Empty;
	}

	uint8_t* pixels = nullptr;
	uint32_t width = 0, height = 0;

	if (lodepng_decode32(&pixels, &width, &height, &data[0], data.size()) != 0) {
		if (pixels != nullptr) free(pixels);
		return Texture2D::Empty;
	}

	auto texture = Texture2D::Create(pixels, width, height);
	free(pixels);
	return std::move(texture);
}
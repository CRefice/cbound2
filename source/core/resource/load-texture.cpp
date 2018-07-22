#include <cstdint>

#include <glad/glad.h>
#include <stb/image.h>

#include "common/logging.hpp"

#include "load-texture.hpp"

Texture load_texture(const std::string& path) {
	DEBUG_LOG("Loading texture: {}", path);
	int width, height, bpp;
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &bpp, 0);
	if (data == nullptr) {
		ERROR_LOG("Unable to load texture {}", path);
	}
	Texture tex(ssm::ivec2(width, height), data);
	free(data);
	DEBUG_LOG("Loaded texture: {}", path);
	return tex;
}

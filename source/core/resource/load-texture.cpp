#include <cstdint>

#include <glad/glad.h>
#include <stb/image.h>

#include "common/logging.hpp"

#include "load-texture.hpp"

namespace fs = std::filesystem;

Texture load_texture(const fs::path& path) {
	DEBUG_LOG("Loading texture: {}", path.string());
	int width, height, bpp;
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &bpp, 0);
	if (data == nullptr) {
		ERROR_LOG("Unable to load texture {}", path.string());
	}
	Texture tex(ssm::ivec2(width, height), data);
	free(data);
	DEBUG_LOG("Loaded texture: {}", path.string());
	return tex;
}

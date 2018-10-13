#include <cstdint>

#include <glad/glad.h>
#include <stb/image.h>

#include "common/logging.hpp"

#include "resource/load-file.hpp"

namespace fs = std::filesystem;
using namespace render;

Texture load_texture(const fs::path& path) {
	SCOPE_LOG("Loading texture: {}", path.string());
	int width, height, channels;
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (data == nullptr) {
		ERROR_LOG("Unable to load texture {}", path.string());
	}
	static const GLenum formats[] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
	TextureData tex_data = { data, formats[channels] };
	Texture tex(ssm::ivec2(width, height), tex_data);
	free(data);
	return tex;
}

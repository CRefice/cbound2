#include <cstdint>

#include <glad/glad.h>
#ifndef NDEBUG
#define STBI_FAILURE_USERMSG
#endif
#include <stb/image.h>

#include "common/logging.hpp"
#include "render/texture.hpp"

#include "resource/resource.hpp"

namespace fs = std::filesystem;
using namespace render;

// A monochrome magenta texture,
// used to quickly debug a missing texture.
Texture default_texture() {
  ssm::vector<uint8_t, 3> magenta(0xFF, 0x00, 0xFF);
  TextureData tex_data = {ssm::data_ptr(magenta), GL_RGB};
  return Texture(ssm::ivec2(1), tex_data);
}

template <>
Texture load_resource<Texture>(const fs::path& path) {
  SCOPE_LOG("Loading texture file \"{}\"", path.string());
  int width, height, channels;
  uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (data == nullptr) {
    ERROR_LOG("Unable to load texture file \"{}\": {}", path.string(),
              stbi_failure_reason());
    return default_texture();
  }
  static const GLenum formats[] = {0, GL_RED, GL_RG, GL_RGB, GL_RGBA};
  TextureData tex_data = {data, formats[channels]};
  Texture tex(ssm::ivec2(width, height), tex_data);
  free(data);
  return tex;
}

template Texture load_resource<Texture>(const fs::path& path);

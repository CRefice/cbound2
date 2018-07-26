#pragma once

#include "object.hpp"

namespace gl {
struct BufferTrait {
  static inline void create(GLuint* handle) { glGenBuffers(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteBuffers(1, handle); }
};
struct TextureTrait {
  static inline void create(GLuint* handle) { glGenTextures(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteTextures(1, handle); }
};
struct VertexArrayTrait {
  static inline void create(GLuint* handle) { glGenVertexArrays(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteVertexArrays(1, handle); }
};

using BufferObject = Object<BufferTrait>;
using TextureObject = Object<TextureTrait>;
using VertexArrayObject = Object<VertexArrayTrait>;
} // namespace gl

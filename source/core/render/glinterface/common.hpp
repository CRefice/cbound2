#pragma once

#include "object.hpp"

namespace gl {
struct BufferTrait {
  static inline void create(GLuint* handle) { glGenBuffers(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteBuffers(1, handle); }
};
struct FrameBufferTrait {
  static inline void create(GLuint* handle) { glGenFramebuffers(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteFramebuffers(1, handle); }
};
struct TextureTrait {
  static inline void create(GLuint* handle) { glGenTextures(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteTextures(1, handle); }
};
struct VertexArrayTrait {
  static inline void create(GLuint* handle) { glGenVertexArrays(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteVertexArrays(1, handle); }
};

using Buffer = Object<BufferTrait>;
using FrameBuffer = Object<FrameBufferTrait>;
using Texture = Object<TextureTrait>;
using VertexArray = Object<VertexArrayTrait>;
} // namespace gl

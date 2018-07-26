#pragma once

#include "object.hpp"

namespace gl {
struct FrameBufferTrait {
  static inline void create(GLuint* handle) { glGenFramebuffers(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteFramebuffers(1, handle); }
};
struct RenderBufferTrait {
  static inline void create(GLuint* handle) { glGenRenderbuffers(1, handle); }
  static inline void destroy(const GLuint* handle) { glDeleteRenderbuffers(1, handle); }
};

using FrameBufferObject = Object<FrameBufferTrait>;
using RenderBufferObject = Object<RenderBufferTrait>;
} // namespace gl

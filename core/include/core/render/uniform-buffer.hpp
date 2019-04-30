#pragma once

#include <string_view>

#include <ssm/matrix.hpp>

#include "glinterface/common.hpp"

namespace render {
class UniformBuffer {
public:
  UniformBuffer(GLuint size);

  auto& handle() { return handl; }
  const auto& handle() const { return handl; }

  GLint bind_point() const { return bind; }

  template <typename T>
  void store(T value, GLuint offset = 0) {
    store_bytes(&value, sizeof(value), offset);
  }

  void store_bytes(void* ptr, GLuint size, GLuint offset = 0);

private:
  gl::BufferObject handl;
  GLint bind;
};
} // namespace render

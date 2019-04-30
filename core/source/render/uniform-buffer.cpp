#include <string>

#include "common/logging.hpp"

#include "render/uniform-buffer.hpp"

namespace render {
static GLuint num_binds = 0;
UniformBuffer::UniformBuffer(GLuint size) : bind(num_binds++) {
  glBindBuffer(GL_UNIFORM_BUFFER, handl);
  glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, bind, handl);
}

void UniformBuffer::store_bytes(void* ptr, GLuint size, GLuint offset) {
  glBindBuffer(GL_UNIFORM_BUFFER, handl);
  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, ptr);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
} // namespace render

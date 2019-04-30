#include <string>

#include "common/logging.hpp"

#include "render/shader.hpp"

namespace shader {
void Stage::compile(std::string_view source) {
  const GLchar* data = source.data();
  const GLint length = source.length();
  glShaderSource(handl, 1, &data, &length);
  glCompileShader(handl);
}

void Program::link() {
  SCOPE_LOG("Linking shader program");
  glLinkProgram(handl);
}

Uniform Program::uniform(const char* name) {
  return Uniform(glGetUniformLocation(handl, name));
}

void Program::bind(const char* name, const render::UniformBuffer& buffer) {
	GLuint index = glGetUniformBlockIndex(handl, name);
	glUniformBlockBinding(handl, index, buffer.bind_point());
}
} // namespace shader

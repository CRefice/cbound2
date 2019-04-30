#pragma once

#include <string_view>

#include <ssm/matrix.hpp>

#include "glinterface/shader.hpp"
#include "uniform-buffer.hpp"

namespace shader {
// The type of shader stage
enum class Type : GLenum {
  Vertex = GL_VERTEX_SHADER,
  Fragment = GL_FRAGMENT_SHADER
};

// A wrapper for ShaderObject,
// that is compiled on construction.
class Stage {
public:
  // Constructs and compiles the shader object.
  Stage(Type type, std::string_view source) : handl(static_cast<GLenum>(type)) {
    compile(source);
  }

  auto& handle() { return handl; }
  const auto& handle() const { return handl; }

private:
  void compile(std::string_view source);

  gl::ShaderObject handl;
};

class Uniform {
public:
  Uniform& operator=(float val) {
    glUniform1f(handle, val);
    return *this;
  }
  Uniform& operator=(const ssm::vec2& val) {
    glUniform2fv(handle, 1, ssm::data_ptr(val));
    return *this;
  }
  Uniform& operator=(const ssm::vec3& val) {
    glUniform3fv(handle, 1, ssm::data_ptr(val));
    return *this;
  }
  Uniform& operator=(const ssm::vec4& val) {
    glUniform4fv(handle, 1, ssm::data_ptr(val));
    return *this;
  }

  Uniform& operator=(int val) {
    glUniform1i(handle, val);
    return *this;
  }
  Uniform& operator=(const ssm::ivec2& val) {
    glUniform2iv(handle, 1, ssm::data_ptr(val));
    return *this;
  }
  Uniform& operator=(const ssm::ivec3& val) {
    glUniform3iv(handle, 1, ssm::data_ptr(val));
    return *this;
  }
  Uniform& operator=(const ssm::ivec4& val) {
    glUniform4iv(handle, 1, ssm::data_ptr(val));
    return *this;
  }

  Uniform& operator=(const ssm::mat2& val) {
    glUniformMatrix2fv(handle, 1, false, ssm::data_ptr(val));
    return *this;
  }
  Uniform& operator=(const ssm::mat3& val) {
    glUniformMatrix3fv(handle, 1, false, ssm::data_ptr(val));
    return *this;
  }
  Uniform& operator=(const ssm::mat4& val) {
    glUniformMatrix4fv(handle, 1, false, ssm::data_ptr(val));
    return *this;
  }

private:
  friend class Program;

  Uniform(GLint handle) : handle(handle) {}
  GLint handle;
};

// A wrapper for Program object,
// that is linked on construction.
class Program {
public:
  template <typename... Stages>
  Program(const Stages&... stages) {
    attach(stages...);
    link();
    detach(stages...);
  }

  Uniform uniform(const char* name);
  void bind(const char* name, const render::UniformBuffer& buffer);

  auto& handle() { return handl; }
  const auto& handle() const { return handl; }

private:
  template <typename... Stages>
  void attach(const Stage& stage, const Stages&... args) {
    glAttachShader(handl, stage.handle());
    attach(args...);
  }
  template <typename... Stages>
  void detach(const Stage& stage, const Stages&... args) {
    glDetachShader(handl, stage.handle());
    detach(args...);
  }
  void attach() {}
  void detach() {}

  void link();

  gl::ProgramObject handl;
};
} // namespace shader

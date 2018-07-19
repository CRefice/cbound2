#pragma once

#include <utility>

#include "object.hpp"

namespace gl {
// Have to emulate other features of Object
// because of different interface
class ShaderObject
{
public:
	ShaderObject(GLenum shader_stage) {
		handle = glCreateShader(shader_stage);
	}
	ShaderObject(const ShaderObject&) = delete;
	ShaderObject(ShaderObject&& obj) {
		*this = std::move(obj);
	}
	~ShaderObject() {
		glDeleteShader(handle);
	}

	ShaderObject& operator=(ShaderObject&& obj) {

		handle = obj.handle;
		obj.handle = 0;
		return *this;
	}
	auto& operator=(const ShaderObject&) = delete;

	operator GLuint() const { return handle; }

private:
	GLuint handle;
};

struct ProgramTrait
{
	static inline void create(GLuint* handle) {
		*handle = glCreateProgram();
	}
	static inline void destroy(const GLuint* handle) {
		glDeleteProgram(*handle);
	}
};
using ProgramObject = Object<ProgramTrait>;
}

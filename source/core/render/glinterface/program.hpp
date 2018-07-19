#pragma once

#include "object.hpp"

namespace gl {
struct ProgramTrait
{
	static inline GLuint create() {
		return glCreateProgram();
	}
	static inline void destroy(GLuint handle) {
		glDeleteProgram(handle);
	}
};

using Program = Object<ProgramTrait>;
}

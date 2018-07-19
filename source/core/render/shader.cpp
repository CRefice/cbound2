#include <string>

#include "common/logging.hpp"

#include "shader.hpp"

namespace shader {
void Stage::compile(std::string_view source) {
	const GLchar* data = source.data();
	const GLint length = source.length();
	glShaderSource(handl, 1, &data, &length);
	glCompileShader(handl);
#ifndef NDEBUG
	GLint success = 0;
	glGetShaderiv(handl, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint length = 0;
		glGetShaderiv(handl, GL_INFO_LOG_LENGTH, &length);
		auto info_log = new GLchar[length];
		glGetShaderInfoLog(handl, length, nullptr, info_log);
		ERROR_LOG("Shader compilation error: {}", info_log);
		delete[] info_log;
	}
#endif
}

void Program::link() {
	glLinkProgram(handl);
#ifndef NDEBUG
	GLint success = 0;
	glGetProgramiv(handl, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint length = 0;
		glGetProgramiv(handl, GL_INFO_LOG_LENGTH, &length);
		auto info_log = new GLchar[length];
		glGetProgramInfoLog(handl, length, nullptr, info_log);
		ERROR_LOG("Program link error: {}", info_log);
		delete[] info_log;
	}
#endif
}
}

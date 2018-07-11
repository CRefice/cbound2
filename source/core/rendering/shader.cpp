#include <gl/core.h>
#include <glm/gtc/type_ptr.hpp>

#include "common/logging.hpp"

#include "shader.hpp"

Shader::Uniform Shader::uniform(const std::string& name) {
	int location = glGetUniformLocation(handle, name.c_str());
	if (location == -1) {
		WARN_LOG("Shader uniform \"", name, "\" does not exist!");
	}
	return Uniform(location);
}

Shader::Uniform& Shader::Uniform::operator=(int val) {
	glUniform1i(location, val);
}

Shader::Uniform& Shader::Uniform::operator=(unsigned val) {
	glUniform1ui(location, val);
}

Shader::Uniform& Shader::Uniform::operator=(float val) {
	glUniform1f(location, val);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::vec2& val) {
	glUniform2f(location, val.x, val.y);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::vec3& val) {
	glUniform3f(location, val.x, val.y, val.z);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::vec4& val) {
	glUniform4f(location, val.x, val.y, val.z, val.w);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::vec4& val) {
	glUniform4f(location, val.x, val.y, val.z, val.w);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::ivec2& val) {
	glUniform2i(location, val.x, val.y);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::ivec3& val) {
	glUniform3i(location, val.x, val.y, val.z);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::ivec4& val) {
	glUniform4i(location, val.x, val.y, val.z, val.w);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::ivec4& val) {
	glUniform4i(location, val.x, val.y, val.z, val.w);
}

Shader::Uniform& Shader::Uniform::operator=(const glm::mat4& val) {
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(val));
}

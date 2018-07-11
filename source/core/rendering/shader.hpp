#pragma once

#include <string>

#include "glinterface\globject.hpp"
#include "glinterface\glprogram.hpp"

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

class Shader
{
public:
	// The Uniform class doesn't offer any validation methods.
	// If a generated uniform is invalid, 
	// or the wrong argument is passed to operator=,
	// the program variables will remain unchanged,
	// and an error will be logged.
	class Uniform
	{
	public:
		Uniform& operator=(int val);
		Uniform& operator=(unsigned val);
		Uniform& operator=(float val);
		Uniform& operator=(const glm::vec2& val);
		Uniform& operator=(const glm::vec3& val);
		Uniform& operator=(const glm::vec4& val);
		Uniform& operator=(const glm::ivec2& val);
		Uniform& operator=(const glm::ivec3& val);
		Uniform& operator=(const glm::ivec4& val);
		Uniform& operator=(const glm::mat4& val);

	private:
		// Only shaders can create uniforms
		friend class Shader;
		Uniform(int location);

		int location;
	};

	Uniform uniform(const std::string& name);
	GlObject<GlProgram> handle;
};

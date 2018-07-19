#pragma once

#include <string_view>

#include "glinterface/shader.hpp"

namespace shader {
// The type of shader stage
enum class Type : GLenum
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

// A wrapper for ShaderObject,
// that is compiled on construction.
class Stage
{
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

// A wrapper for Program object,
// that is linked on construction.
class Program
{
public:
	template <typename... Stages>
	Program(const Stages&... stages) {
		attach(stages...);
		link();
		detach(stages...);
	}

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
	void attach() { }
	void detach() { }

	void link();

	gl::ProgramObject handl;
};
}

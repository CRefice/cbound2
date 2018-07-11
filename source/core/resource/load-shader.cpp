#include <gl/core.h>

#include <fstream>
#include <string>
#include <sstream>

#include "common/path-util.hpp"
#include "common/logging.hpp"

#include "load-shader.hpp"

Shader load_shader(const std::pair<std::string, std::string>& paths) {
	const auto& vs_path = paths.first;
	const auto& fs_path = paths.second;

	//Read files into memory
	std::ifstream vfile(vs_path);
	std::ifstream ffile(fs_path);

	std::stringstream vs, fs;
	vs << vfile.rdbuf();
	fs << ffile.rdbuf();
	const auto vsource = vs.str();
	const auto fsource = fs.str();
	const char* vchars = vsource.c_str();
	const char* fchars = fsource.c_str();
		
	//Create and compile shaders
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex, 1, &(vchars), NULL);
	glShaderSource(fragment, 1, &(fchars), NULL);

	//Link shaders
	Shader shader;
	glAttachShader(shader.handle, vertex);
	glAttachShader(shader.handle, fragment);
	glLinkProgram(shader.handle);

#ifndef NDEBUG
	const auto vs_name = path::filename(vs_path);
	const auto fs_name = path::filename(fs_path);
	// Compilation error checking/logging
	{
		char vs_info_log[512];
		char fs_info_log[512];
		int success = 0;

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			WARN_LOG("Vertex shader ", vs_name, " failed to compile!");
		}
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			WARN_LOG("Fragment shader ", fs_name, " failed to compile!");
		}
		glGetShaderInfoLog(vertex, 512, nullptr, vs_info_log);
		glGetShaderInfoLog(fragment, 512, nullptr, fs_info_log);
		INFO_LOG("Info log for vertex shader ", vs_name, ": ", vs_info_log);
		INFO_LOG("Info log for fragment shader ", fs_name, ": ", fs_info_log);
	}
	// Linking error checking/logging
	{
		char link_info_log[512];
		int success = 0;

		glGetProgramiv(shader.handle, GL_LINK_STATUS, &success);
		if (!success) {
			WARN_LOG("Shader compiled from ", vs_name, ", " fs_name ", failed to link!");
		}

		glGetProgramInfoLog(shader.handle, 512, NULL, link_info_log);
		INFO_LOG("Shader linking info log: ", link_info_log);
	}
#endif

	//Cleanup
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shader;
}

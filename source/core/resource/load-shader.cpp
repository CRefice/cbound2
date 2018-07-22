#include <cstdlib>
#include <fstream>

#include "common/logging.hpp"

#include "load-shader.hpp"

shader::Stage load_shader(const std::string& path) {
	DEBUG_LOG("Loading shader: {}", path);
	shader::Type type;
	if (path.rfind(".f") != std::string::npos) type = shader::Type::Fragment;
	else type = shader::Type::Vertex;
	
	std::ifstream file(path, std::ios::ate);
	if (!file.good()) {
		FATAL_LOG("Unable to open file {}", path);
		std::abort();
	}
	auto length = file.tellg();
	file.seekg(0, std::ios::beg);
	auto source = new char[length];
	file.read(source, length);

	shader::Stage stage(type, std::string_view(source, length));
	delete[] source;
	DEBUG_LOG("Loaded shader: {}", path);
	return stage;
}

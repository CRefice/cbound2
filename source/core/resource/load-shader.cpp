#include <cstdlib>
#include <fstream>

#include "common/logging.hpp"

#include "load-shader.hpp"

namespace fs = std::filesystem;

shader::Stage load_shader(const fs::path& path) {
	DEBUG_LOG("Loading shader: {}", path.string());
	shader::Type type;
	if (path.string().rfind(".f") != std::string::npos) type = shader::Type::Fragment;
	else type = shader::Type::Vertex;
	
	std::ifstream file(path, std::ios::ate);
	if (!file.good()) {
		ERROR_LOG("Unable to open file {}", path.string());
		std::abort();
	}
	auto length = file.tellg();
	file.seekg(0, std::ios::beg);
	auto source = new char[length];
	file.read(source, length);

	shader::Stage stage(type, std::string_view(source, length));
	delete[] source;
	DEBUG_LOG("Loaded shader: {}", path.string());
	return stage;
}

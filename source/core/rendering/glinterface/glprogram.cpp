#include <gl\core.h>

#include "glprogram.hpp"

typename GlProgram::value_type GlProgram::create() {
	return glCreateProgram();
}

void GlProgram::destroy(typename GlProgram::value_type val) {
	glDeleteProgram(val);
}

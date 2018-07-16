#include <glad/glad.h>

#include "gltexture.hpp"

typename GlTexture::value_type GlTexture::create() {
	unsigned val;
	glGenTextures(1, &val);
	return val;
}

void GlTexture::destroy(typename GlTexture::value_type val) {
	glDeleteTextures(1, &val);
}

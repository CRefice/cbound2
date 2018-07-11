#include <gl\core.h>
#include <png.h>

#include "load-image.hpp"

static void load_png() {
	
}

Image load_image(const std::string& path) {
	int width, height, bpp;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &bpp, 0);
	if(!data) return Texture{};

	Texture tex;
	tex.size.x = width;
	tex.size.y = height;
	tex.frame_count = 0;

	glBindTexture(GL_TEXTURE_2D_ARRAY, tex.handle);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, 1);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	free(data);
	return tex;
}

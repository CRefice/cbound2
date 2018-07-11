#pragma once

#include "common/vectors.hpp"

#include "palette.hpp"

class Image
{
public:
	enum class Format { Rgb, Paletted };
	struct Info
	{
		Format format;
		vec2<int> size;
	};

	Image(Info information) : info(information) {}

	Format format() const { return info.format; }
	vec2<int> size() const { return info.size; }
	Info information() const { return info; }

	// Palette goes unused if format isn't paletted,
	// but it's only a minimal waste of memory (4 bytes).
	Palette palette;
	GlObject<GlTexture> handle;

private:
	const Info info;
};

#pragma once

#include <string>

#include "texture.hpp"

#include "core/rendering/tileset.hpp"

class TileSetLoader
{
public:
	TileSetLoader(TextureLoader& loader) : loader(loader) {}

	using res_type = std::string;
	using res_type = TileSet;

	res_type load(const key_type& res_name);

private:
	TextureLoader& tex_loader;
};

#include <tinyxml2/tinyxml2.h>
#include <common/path-util.hpp>

#include "tileset.hpp"

typename TileSetLoader::res_type TileSetLoader::load(const std::string& path) {
	XMLDocument doc;
	doc.LoadFile(path.c_str());
	auto tileset_element = doc.FirstChildElement("tileset");

	TileSet tileset;
	tileset_element->FindAttribute("tilewidth")->QueryIntValue(&tileset.tile_size.x);
	tileset_element->FindAttribute("tileheight")->QueryIntValue(&tileset.tile_size.y);

	//TSX format stores columns and tile count, not rows
	uint tilecount;
	tileset_element->FindAttribute("tilecount")->QueryIntValue(&tilecount);
	tileset_element->FindAttribute("columns")->QueryIntValue(&tileset.size.y);
	tileset.size.x = tilecount / tileset.size.y;

	auto image_element = tileset_element.FirstChildElement("image");
	tileset.texture = texloader.load(path::user_path(DIR_TEXTURES)
			+ tileset_element->Findattribute("source")->Value());
}

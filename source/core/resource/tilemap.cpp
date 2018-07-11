#include <fstream>

#include "tilemap.hpp"

typename TileMapLoader::res_type TileMapLoader::load(const std::string& path) {
	std::ifstream file{path};

	std::string header;
	header.resize(5);
	file.read(&header[0], 5);
	if (header != "CTLMP") throw; //TODO

	TileMap tilemap;
	file >> tilemap.map_size.x;
	file >> tilemap.map_size.y;
	file >> tilemap.tile_size;
	tilemap.tiles.resize(tilemap.map_size.x * tilemap.map_size.y);

	for (auto& tilevec : tilemap.tiles) {
		file >> tilevec.x;
		file >> tilevec.y;
	}
	return tilemap;
}

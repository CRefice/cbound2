#pragma once

#include <string>

#include "core/common/tilemap.hpp"

class TileMapLoader
{
public:
	using std::string = key_type;
	using TileMap = res_type;

	res_type load(const key_type& res_name);
};

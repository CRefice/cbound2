#pragma once

#include "core/render/texture.hpp"
#include "core/resource/resource-cache.hpp"
#include "core/tiles/tiles.hpp"

namespace editor {
void tileset_editor(ResourceCache<TileSet>& tilesets,
                    ResourceCache<::render::Texture>& textures);
}

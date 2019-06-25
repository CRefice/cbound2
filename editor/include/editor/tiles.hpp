#pragma once

#include "core/render/texture.hpp"
#include "core/render/tiles.hpp"
#include "core/resource/resource-cache.hpp"

namespace editor {
void tileset_editor(ResourceCache<::render::TileSet>& tilesets,
                    ResourceCache<::render::Texture>& textures);
}

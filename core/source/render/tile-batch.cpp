#include <vector>

#include <ssm/transform.hpp>

#include "common/logging.hpp"

#include "render/tex-coords.hpp"
#include "render/tile-batch.hpp"

namespace render {
static const GLbitfield mapflags = GL_MAP_WRITE_BIT |
                                   GL_MAP_INVALIDATE_BUFFER_BIT |
                                   GL_MAP_INVALIDATE_RANGE_BIT;

// Representation of tile vertices in memory
struct TileVertex {
  ssm::vec3 pos;
  TexCoord uv;
};

// Fill vertex and index buffers
static void fill_buffers(std::vector<TileVertex> &vertices,
                         std::vector<GLushort> &indices, const TileMap &map,
                         const TileSet &set, const ssm::ivec2 tex_size) {
  for (const auto &layer : map.layers) {
    const auto z = layer.depth;
    for (std::size_t i = 0; i < layer.tiles.size(); ++i) {
      int tile = layer.tiles[i];
      if (tile == 0)
        continue;
      // since 0 is null, everything is shifted by 1
      tile--;
      // Tiles go top-right, so we invert the position (as in our coord system
      // it starts from bottom)
      const ssm::vec2 tile_pos(i % map.size.x, map.size.y - i / map.size.x);
      const ssm::vec2 pos = tile_pos * map.tile_size;
      const Rectangle<float> bounds(
          pos, pos + ssm::vec2(map.tile_size.x, -map.tile_size.y));
      const auto frame = frame_from_id(set, tile);
      const GLushort base_index = vertices.size();
      vertices.push_back({ssm::extend(bounds.top_left(), z),
                          normalize(frame.top_left(), tex_size)});
      vertices.push_back({ssm::extend(bounds.bottom_left(), z),
                          normalize(frame.bottom_left(), tex_size)});
      vertices.push_back({ssm::extend(bounds.bottom_right(), z),
                          normalize(frame.bottom_right(), tex_size)});
      vertices.push_back({ssm::extend(bounds.top_right(), z),
                          normalize(frame.top_right(), tex_size)});

      indices.emplace_back(base_index);
      indices.emplace_back(base_index + 1);
      indices.emplace_back(base_index + 2);
      indices.emplace_back(base_index);
      indices.emplace_back(base_index + 2);
      indices.emplace_back(base_index + 3);
    }
  }
}

// Fill position, uv, and index buffers.
static void fill_anim_buffers(std::vector<ssm::vec3> &positions,
                              std::vector<int> &ids,
                              std::vector<GLushort> &indices,
                              const TileMap &map, const TileSet &set) {
  for (const auto &layer : map.layers) {
    const auto z = layer.depth;
    for (std::size_t i = 0; i < layer.tiles.size(); ++i) {
      int tile = layer.tiles[i] - 1;
      if (set.anim_tiles.find(tile) == set.anim_tiles.end())
        continue;
      ids.push_back(tile);
      const ssm::vec2 tile_pos(i % map.size.x, map.size.y - i / map.size.x);
      const ssm::vec2 pos = tile_pos * map.tile_size;
      const Rectangle<float> bounds(
          pos, pos + ssm::vec2(map.tile_size.x, -map.tile_size.y));
      const GLushort base_index = positions.size();
      positions.push_back(ssm::extend(bounds.top_left(), z));
      positions.push_back(ssm::extend(bounds.bottom_left(), z));
      positions.push_back(ssm::extend(bounds.bottom_right(), z));
      positions.push_back(ssm::extend(bounds.top_right(), z));
      indices.emplace_back(base_index);
      indices.emplace_back(base_index + 1);
      indices.emplace_back(base_index + 2);
      indices.emplace_back(base_index);
      indices.emplace_back(base_index + 2);
      indices.emplace_back(base_index + 3);
    }
  }
}

StaticTileBatch::StaticTileBatch(ResourceCache<Texture> &resources,
                                 const TileMap &map, const TileSet &set)
    : texture(resources.load(set.texture_id)) {
  std::vector<TileVertex> vertices;
  std::vector<GLushort> indices;
  fill_buffers(vertices, indices, map, set, texture->size());

  index_count = indices.size();

  glBindVertexArray(vao);
  gl::BufferObject vbo, ebo;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort),
               indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TileVertex),
               vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TileVertex),
                        reinterpret_cast<void *>(offsetof(TileVertex, pos)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(TileVertex),
                        reinterpret_cast<void *>(offsetof(TileVertex, uv)));
  glBindVertexArray(0);
}

void StaticTileBatch::issue_draw_call() {
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, texture->handle());
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT,
                 reinterpret_cast<void *>(0));
}

AnimTileBatch::AnimTileBatch() : uvs(GL_ARRAY_BUFFER, 0) {}

AnimTileBatch::AnimTileBatch(ResourceCache<Texture> &resources,
                             const TileMap &map, const TileSet &set)
    : texture(resources.load(set.texture_id)),
      uvs(GL_ARRAY_BUFFER, map.size.x * map.size.y * 6) {
  for (const auto &[id, sequence] : set.anim_tiles) {
    sequencers.emplace(id, sequence);
  }
  std::vector<ssm::vec3> positions;
  std::vector<GLushort> indices;
  fill_anim_buffers(positions, ids, indices, map, set);

  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind_buf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort),
               indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(ssm::vec3),
               positions.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ssm::vec3),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
  glBufferData(GL_ARRAY_BUFFER, uvs.capacity() * sizeof(TexCoord), nullptr,
               GL_STREAM_DRAW);
  glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(TexCoord),
                        reinterpret_cast<void *>(0));
  uvs.map(mapflags);
  glBindVertexArray(0);
}

void AnimTileBatch::progress(double dt) {
  for (auto &[id, sequencer] : sequencers) {
    sequencer.progress(dt);
  }
}

void AnimTileBatch::issue_draw_call(const TileSet &set) {
  uvs.clear();
  for (int id : ids) {
    auto seq = sequencers.find(id);
    if (seq == sequencers.end()) {
      WARN_LOG("No data found for id {}", id);
    }
    TexFrame frame = frame_from_id(set, seq->second.current_value());
    uvs.push_back(normalize(frame.top_left(), texture->size()));
    uvs.push_back(normalize(frame.bottom_left(), texture->size()));
    uvs.push_back(normalize(frame.bottom_right(), texture->size()));
    uvs.push_back(normalize(frame.top_right(), texture->size()));
  }

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buf);
  uvs.unmap();

  glBindTexture(GL_TEXTURE_2D, texture->handle());
  glDrawElements(GL_TRIANGLES, ids.size() * 6, GL_UNSIGNED_SHORT,
                 reinterpret_cast<void *>(0));

  uvs.map(mapflags);
}
}

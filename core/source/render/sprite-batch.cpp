#include <cmath>
#include <limits>

#include <ssm/transform.hpp>

#include "common/rectangle.hpp"

#include "render/sprite-batch.hpp"

static const GLbitfield mapflags = GL_MAP_WRITE_BIT |
                                   GL_MAP_INVALIDATE_BUFFER_BIT |
                                   GL_MAP_INVALIDATE_RANGE_BIT;

namespace render {
SpriteBatch::SpriteBatch(size_type batch_size,
                         ResourceCache<Texture>& resources)
    : resources(resources), batch_texture(nullptr, ""),
      vertices(GL_ARRAY_BUFFER, batch_size * 4),
      indices(GL_ELEMENT_ARRAY_BUFFER, batch_size * 6) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

  // Initialize the vertex and element buffer data
  // For every sprite there are 4 verts and 6 indices.
  glBufferData(GL_ARRAY_BUFFER, batch_size * 4 * sizeof(SpriteVertex), nullptr,
               GL_STREAM_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch_size * 6 * sizeof(size_type),
               nullptr, GL_STREAM_DRAW);
  vertices.map(mapflags);
  indices.map(mapflags);

  // Vertex format:
  // 0: pos.x pos.y pos.z (floats)
  // 1: uv.x uv.y (in shorts, normalized)
  // 2: col.x col.y col.z col.w (floats)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
                        reinterpret_cast<void*>(offsetof(SpriteVertex, pos)));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(SpriteVertex),
                        reinterpret_cast<void*>(offsetof(SpriteVertex, uv)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
                        reinterpret_cast<void*>(offsetof(SpriteVertex, color)));

  glBindVertexArray(0);
}

void SpriteBatch::draw(const Sprite& sprite, const ssm::vec2& pos) {
  if (batch_texture.identifier() != sprite.texture_id) {
    flush();
    batch_texture = resources.load(sprite.texture_id);
  }

  // Winding order: counterclockwise faces front-wards
  const Rectangle<float> bounds(pos, pos + sprite.size);
  const auto size = batch_texture->size();
  const auto frame = sprite.frame;
  const auto color = sprite.color;
  const size_type base_index = vertices.size();
  const float z = sprite.depth;
  vertices.emplace_back(ssm::extend(bounds.top_left(), z),
                        normalize(frame.bottom_left(), size), color);
  vertices.emplace_back(ssm::extend(bounds.bottom_left(), z),
                        normalize(frame.top_left(), size), color);
  vertices.emplace_back(ssm::extend(bounds.bottom_right(), z),
                        normalize(frame.top_right(), size), color);
  vertices.emplace_back(ssm::extend(bounds.top_right(), z),
                        normalize(frame.bottom_right(), size), color);

  indices.push_back(base_index);
  indices.push_back(base_index + 1);
  indices.push_back(base_index + 2);
  indices.push_back(base_index);
  indices.push_back(base_index + 2);
  indices.push_back(base_index + 3);
}

void SpriteBatch::flush() {
  issue_draw_call();
  clear();
}

void SpriteBatch::issue_draw_call() {
  if (indices.size() == 0)
    return;

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
  vertices.unmap();
  indices.unmap();

  glBindTexture(GL_TEXTURE_2D, batch_texture->handle());
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT,
                 reinterpret_cast<void*>(0));

  vertices.map(mapflags);
  indices.map(mapflags);
}

void SpriteBatch::clear() {
  vertices.clear();
  indices.clear();
}
} // namespace render

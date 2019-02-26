#pragma once

#include <hash/flat_hash_map.hpp>

#include "core/anim/sequencer.hpp"
#include "core/render/tex-coords.hpp"

#include "entity.hpp"
#include "renderer.hpp"

namespace ecs {
// Edits sprites in a Renderer based on animation data.
class Animator {
public:
  using Sequencer = anim::Sequencer<render::TexFrame>;
  using iterator = ska::flat_hash_map<EntityId, Sequencer>::iterator;
  using const_iterator =
      ska::flat_hash_map<EntityId, Sequencer>::const_iterator;

  Animator(Renderer& renderer) : renderer(renderer) {}

  void submit(EntityId id, Sequencer anim);

  void update(double dt);

private:
  Renderer& renderer;
  ska::flat_hash_map<EntityId, Sequencer> anims;
};
} // namespace ecs

#pragma once

#include <hash/flat_hash_map.hpp>
#include <sol/forward.hpp>

#include "core/anim/sequencer.hpp"
#include "core/render/tex-coords.hpp"

#include "entity.hpp"

namespace ecs {
// Edits sprites in a Renderer based on animation data.
class Animator {
public:
  using Sequencer = anim::Sequencer<render::TexFrame>;

  void submit(EntityId id, Sequencer anim);

  Sequencer* find(EntityId id);
  const Sequencer* find(EntityId id) const;

  void remove(EntityId id);

  void update(double dt);

  void load_entity(const EntityId& id, sol::table& entity);
  void bind_entity_fields(sol::usertype<EntityId>& meta);

private:
  ska::flat_hash_map<EntityId, Sequencer> anims;
}; // namespace ecs
} // namespace ecs

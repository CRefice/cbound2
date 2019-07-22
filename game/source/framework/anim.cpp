#include "core/render/tex-coords.hpp"

#include "anim.hpp"

namespace fw {
using namespace anim;
using namespace render;

void bind_anim_libs(sol::state_view state) {
  auto table = state.create_table("anim");
  LuaTraits<Sequencer<TexFrame>>::bind(table, "Sequence");
  LuaTraits<PlayMode>::bind(table, "play_mode");
}
} // namespace fw

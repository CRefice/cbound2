#include <sol/sol.hpp>

#include "common/logging.hpp"
#include "core/script/script.hpp"

#include "render.hpp"

namespace fw {
using namespace render;
using Traits = LuaTraits<Sprite>;

std::optional<Sprite> Traits::parse(const sol::table& table) {
  auto image_id = table.get<sol::optional<std::string>>("image");
  auto size = table.get<sol::optional<ssm::vec2>>("size");
  auto frame = table.get<sol::optional<Rectangle<int>>>("frame");
  auto depth = table.get<sol::optional<float>>("depth").value_or(1.0);
  auto color = ssm::vec4(1.0f);
  if (auto clr = table.get<sol::optional<std::string>>("color")) {
    color = script::parse_color(*clr).value_or(color);
  }
  if (!image_id || !size || !frame) {
    WARN_LOG("Invalid sprite data format: missing {}",
             !image_id ? "image" : !size ? "size" : "frame data");
    return {};
  }
  return Sprite{*image_id, *size, *frame, depth, color};
}

void Traits::bind(sol::table& table, const char* name) {
  auto meta = table.new_usertype<Sprite>(name, sol::no_constructor);
  meta["texture"] = &Sprite::texture_id;
  meta["size"] = &Sprite::size;
  meta["frame"] = &Sprite::frame;
  meta["depth"] = &Sprite::depth;
  meta["color"] = &Sprite::color;
}

void bind_render_libs(sol::state_view state) {
  auto table = state.create_table("render");
  Traits::bind(table, "Sprite");
}
} // namespace fw

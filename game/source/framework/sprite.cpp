#include <sol/sol.hpp>

#include "core/script/script.hpp"

#include "common/logging.hpp"

#include "sprite.hpp"

namespace fw::render {
std::optional<::render::Sprite> parse_sprite(const sol::table& table) {
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
  return ::render::Sprite{*image_id, *size, *frame, depth, color};
}
} // namespace fw::render

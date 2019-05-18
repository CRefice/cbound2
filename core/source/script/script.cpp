#include <charconv>
#include <cstdint>

#include "common/paths.hpp"
#include "script/script.hpp"

static bool result_valid(std::from_chars_result result) {
  return result.ec != std::errc::invalid_argument &&
         result.ec != std::errc::result_out_of_range;
}

namespace script {
std::optional<ssm::vec4> parse_color(std::string_view str) {
  if (str[0] != '#' || str.length() < 7)
    return {};
  str.remove_prefix(1);

  std::uint8_t r, g, b, a = 0xFF;
  if (!result_valid(std::from_chars(str.data() + 0, str.data() + 2, r, 16)) ||
      !result_valid(std::from_chars(str.data() + 2, str.data() + 4, g, 16)) ||
      !result_valid(std::from_chars(str.data() + 4, str.data() + 6, b, 16)))
    return {};

  // Parse alpha if available
  if (str.length() == 8) {
    std::from_chars(str.data() + 6, str.data() + 8, a, 16);
  }
  return ssm::vec4(r, g, b, a) / 255.f;
}

sol::state new_environment() {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);

  std::string package_path = lua["package"]["path"];
  lua["package"]["path"] = package_path + (package_path.empty() ? "" : ";") +
                           path::install_dir().string() + "/resources/?.lua";
  load_common_libs(lua);
  return lua;
}

void load_common_libs(sol::state& state) {
  state.new_usertype<ssm::vec2>(
      "Vec2", sol::constructors<ssm::vec2(), ssm::vec2(float, float)>(), "x",
      sol::property([](ssm::vec2& vec) { return static_cast<float>(vec.x); },
                    [](ssm::vec2& vec, float val) { vec.x = val; }),
      "y",
      sol::property([](ssm::vec2& vec) { return static_cast<float>(vec.y); },
                    [](ssm::vec2& vec, float val) { vec.y = val; }),
      "__add", [](ssm::vec2& a, ssm::vec2 b) { return a + b; });
}
} // namespace script

#include <charconv>
#include <cstdint>

#include "common/logging.hpp"
#include "common/paths.hpp"
#include "common/rectangle.hpp"

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
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine);

  std::string package_path = lua["package"]["path"];
  lua["package"]["path"] = package_path + (package_path.empty() ? "" : ";") +
                           path::install_dir().string() + "/resources/?.lua";
  load_common_libs(lua);
  return lua;
}

template <typename T>
static void define_vec2_type(sol::state& state, const char* name) {
  using Vec = ssm::vector<T, 2>;
  state.new_usertype<Vec>(
      name, sol::constructors<Vec(), Vec(T, T)>(), "x",
      sol::property([](Vec& vec) -> T { return vec.x; },
                    [](Vec& vec, T val) { vec.x = val; }),
      "y",
      sol::property([](Vec& vec) -> T { return vec.y; },
                    [](Vec& vec, T val) { vec.y = val; }),
      "__add", [](Vec& a, const Vec& b) { return a + b; }, "__sub",
      [](Vec& a, const Vec& b) { return a - b; }, "__mul",
      sol::overload([](Vec& a, T b) { return a * b; },
                    [](Vec& a, const Vec& b) { return a * b; }),
      "__div",
      sol::overload([](Vec& a, T b) { return a / b; },
                    [](Vec& a, const Vec& b) { return a / b; }));
}

template <typename T>
static void define_rect_type(sol::state& state, const char* name) {
  using Rect = Rectangle<T>;
  state.new_usertype<Rect>(
      name,
      sol::constructors<Rect(), Rect(ssm::vector<T, 2>, ssm::vector<T, 2>),
                        Rect(T, T, T, T)>(),
      "left", &Rect::left, "right", &Rect::right, "top", &Rect::top, "bottom",
      &Rect::bottom, "width", &Rect::width, "height", &Rect::height,
      "bottom_left", &Rect::bottom_left, "top_right", &Rect::top_right, "size",
      &Rect::size, "center", &Rect::center, "translate", &translate<T>);
}

void load_common_libs(sol::state& state) {
  define_vec2_type<float>(state, "Vec2");
  define_vec2_type<int>(state, "IVec2");
  define_rect_type<float>(state, "Rect");
  define_rect_type<int>(state, "IRect");
}

sol::protected_function_result on_error(lua_State*,
                                        sol::protected_function_result pfr) {
  sol::error err = pfr;
  ERROR_LOG("Lua error: {}", err.what());
  return pfr;
}
} // namespace script

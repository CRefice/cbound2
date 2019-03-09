#include "script/script.hpp"

namespace script {
sol::state new_environment() {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);
	load_common_libs(lua);
  return lua;
}

void load_common_libs(sol::state& state) {
  state.new_usertype<ssm::vec2>(
      "Vec2", "x",
      sol::property([](ssm::vec2 vec) { return static_cast<float>(vec.x); },
                    [](ssm::vec2 vec, float val) { vec.x = val; }),
      "y",
      sol::property([](ssm::vec2 vec) { return static_cast<float>(vec.y); },
                    [](ssm::vec2 vec, float val) { vec.y = val; }));
}
} // namespace script

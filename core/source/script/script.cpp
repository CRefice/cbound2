#include "common/paths.hpp"
#include "script/script.hpp"

namespace script {
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

#define SOL_CHECK_ARGUMENTS 1

#include "ui/text.hpp"

#include "ui.hpp"

namespace fw::ui {
using namespace ::ui;

void load_libraries(sol::state& state) {
  state.new_usertype<Widget>("Widget", "new", sol::no_constructor);
  state.new_usertype<Text>(
      "Text",
      sol::constructors<Text(std::string, ssm::vec2),
                        Text(std::string, double, ssm::vec2)>(),
      sol::base_classes, sol::bases<Widget>());
}
} // namespace fw::ui

#include <memory>
#include <string_view>

#include <sol/sol.hpp>

#include "common/logging.hpp"
#include "core/script/script.hpp"

#include "ui/text.hpp"
#include "ui/window.hpp"

#include "ui.hpp"

namespace fw {
using namespace ui;

void LuaTraits<Widget>::bind(sol::table& tbl, const char* name) {
  auto meta = tbl.new_usertype<Widget>(name, sol::no_constructor);
  meta["size"] = &Widget::size;
}

void LuaTraits<Text>::bind(sol::table& tbl, const char* name) {
  auto meta = tbl.new_usertype<Text>(
      name,
      sol::factories(
          [](std::string_view text, ssm::vec2 size) {
            return std::make_shared<Text>(text, size);
          },
          [](std::string_view text, double speed, ssm::vec2 size) {
            return std::make_shared<Text>(text, speed, size);
					},
          [](std::string_view text, ssm::vec2 size, std::string font) {
            return std::make_shared<Text>(text, size, std::move(font));
          },
          [](std::string_view text, double speed, ssm::vec2 size,
             std::string font) {
            return std::make_shared<Text>(text, speed, size, std::move(font));
          }),
      sol::base_classes, sol::bases<Widget>());

  meta["skip"] = &Text::skip;
  meta["done"] = &Text::done;
}

void LuaTraits<Window>::bind(sol::table& tbl, const char* name) {
  tbl.new_usertype<Window>(name, sol::factories([](ssm::vec2 size) {
                             return std::make_shared<Window>(size);
                           }),
                           sol::base_classes, sol::bases<Widget>());
}

void bind_ui_libs(sol::state_view state) {
  auto table = state.create_named_table("ui");
  LuaTraits<Widget>::bind(table, "Widget");
  LuaTraits<Text>::bind(table, "Text");
  LuaTraits<Window>::bind(table, "Window");
}
} // namespace fw

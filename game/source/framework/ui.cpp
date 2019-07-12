#include <sol/sol.hpp>

#include "core/script/script.hpp"
#include "common/logging.hpp"

#include "ui/text.hpp"
#include "ui/window.hpp"

#include "ui.hpp"

using namespace std::literals;

using namespace render;
using namespace ui;

namespace fw::ui {
std::unique_ptr<Widget> parse_text_widget(const sol::table& table,
                                          ssm::vec2 size) {
  auto text = table.get_or("text", ""s);
  auto font = table.get_or("font", "fonts/font.fnt"s);

  if (auto speed = table.get<sol::optional<double>>("speed")) {
    return std::make_unique<Text>(text, *speed, size, font);
  } else {
    auto widget = std::make_unique<Text>(text, 0.02, size, font);
    widget->skip();
    return widget;
  }
}

std::unique_ptr<Widget> parse_widget(const sol::table& table) {
  ssm::vec2 size;
  auto sz_tbl = table["size"];
  if (!sz_tbl) {
    ERROR_LOG("Ui data does not contain size!");
    return nullptr;
  }
  if (auto sz = script::parse_vec2<float>(sz_tbl)) {
    size = *sz;
  } else {
    ERROR_LOG("Ui data does not contain size!");
    return nullptr;
  }

  std::string kind = table.get_or("kind", ""s);
  if (kind == "text"s) {
    return parse_text_widget(table, size);
  } else if (kind == "window"s) {
    return std::make_unique<Window>(size);
  } else {
    ERROR_LOG("Unknown UI widget kind!");
    return nullptr;
	}
}
} // namespace fw::ui

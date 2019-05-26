#include <sol/sol.hpp>

#include "ui/text.hpp"
#include "ui/ui.hpp"
#include "ui/window.hpp"

#include "ui-manager.hpp"

using namespace render;
using namespace ui;

namespace ecs {
void UiManager::submit(EntityId id, ui::Widget *widget) {
  widgets.insert_or_assign(id, widget);
}

void UiManager::remove(EntityId id) { widgets.erase(id); }

void UiManager::update(double dt) {
  for (auto &[key, widget] : widgets) {
    widget->update(dt);
  }
}

void UiManager::draw_all(const Scene &scene) {
  auto context = ui::UiContext{batch};
  for (auto &[id, widget] : widgets) {
    auto mvmt = scene.find(id);
    widget->draw(mvmt->pos, context);
  }
  batch.flush();
}

void UiManager::load_libraries(sol::state &state) {
  auto table = state.create_table("ui");
  table.new_usertype<Widget>("Widget", "new", sol::no_constructor);
  table.new_usertype<Text>(
      "Text", "new",
      sol::overload(
          [this](std::string_view txt, ssm::vec2 sz) {
            return Text(txt, sz, fonts.load("fonts/font.fnt"));
          },
          [this](std::string_view txt, ssm::vec2 sz, const std::string &font) {
            return Text(txt, sz, fonts.load(font));
          },
          [this](std::string_view txt, double speed, ssm::vec2 sz) {
            return Text(txt, speed, sz, fonts.load("fonts/font.fnt"));
          },
          [this](std::string_view txt, double speed, ssm::vec2 sz,
                 const std::string &font) {
            return Text(txt, speed, sz, fonts.load(font));
          }),
      "done", &Text::done, "skip", &Text::skip, sol::base_classes,
      sol::bases<Widget>());

  table.new_usertype<Window>("Window", sol::constructors<Window(ssm::vec2)>(),
                             sol::base_classes, sol::bases<Widget>());
}
} // namespace ecs

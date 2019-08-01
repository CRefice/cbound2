#include <memory>

#include <sol/sol.hpp>

#include "common/logging.hpp"

#include "ui/text.hpp"
#include "ui/ui.hpp"
#include "ui/window.hpp"

#include "ui-manager.hpp"

using namespace render;
using namespace ui;

namespace ecs {
void UiManager::submit(EntityId id, std::shared_ptr<ui::Widget> widget) {
  widgets.insert_or_assign(id, std::move(widget));
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
    if (mvmt) {
      widget->draw(mvmt->pos, context);
    }
  }
  batch.flush();
}

void UiManager::load_entity(const EntityId &id, sol::table &entity) {
  if (entity["ui"] != sol::nil) {
    std::shared_ptr<Widget> widget = entity["ui"];
    submit(id, std::move(widget));
  }
}

void UiManager::bind_libs(sol::state_view state) {
  auto default_font = fonts.load("fonts/font.fnt");

  auto table = state.create_named_table("ui");
  auto widget = table.new_usertype<Widget>("Widget", sol::no_constructor);
  widget["size"] = &Widget::size;
  auto text = table.new_usertype<Text>(
      "Text",
      sol::factories(
          [default_font](std::string_view text, ssm::vec2 size) {
            return std::make_shared<Text>(text, size, default_font);
          },
          [default_font](std::string_view text, double speed, ssm::vec2 size) {
            return std::make_shared<Text>(text, speed, size, default_font);
          },
          [this](std::string_view text, ssm::vec2 size,
                 const std::string &font) {
            return std::make_shared<Text>(text, size, fonts.load(font));
          },
          [this](std::string_view text, double speed, ssm::vec2 size,
                 const std::string &font) {
            return std::make_shared<Text>(text, speed, size, fonts.load(font));
          }),
      sol::base_classes, sol::bases<Widget>());
  text["skip"] = &Text::skip;
  text["done"] = &Text::done;
  table.new_usertype<Window>("Window", sol::factories([](ssm::vec2 size) {
                               return std::make_shared<Window>(size);
                             }),
                             sol::base_classes, sol::bases<Widget>());
}
} // namespace ecs

#include <sol/sol.hpp>

#include "ui/text.hpp"
#include "ui/ui.hpp"
#include "ui/window.hpp"

#include "framework/ui.hpp"

#include "ui-manager.hpp"

using namespace render;
using namespace ui;

namespace ecs {
void UiManager::submit(EntityId id, std::unique_ptr<ui::Widget> widget) {
  widgets.insert_or_assign(id, std::move(widget));
}

void UiManager::remove(EntityId id) { widgets.erase(id); }

void UiManager::update(double dt) {
  for (auto &[key, widget] : widgets) {
    widget->update(dt);
  }
}

void UiManager::draw_all(const Scene &scene) {
  auto context = ui::UiContext{batch, fonts};
  for (auto &[id, widget] : widgets) {
    auto mvmt = scene.find(id);
    if (mvmt) {
      widget->draw(mvmt->pos, context);
    }
  }
  batch.flush();
}

void UiManager::load_libraries(sol::state &state) {
  auto table = state.create_table("ui");
  table.new_usertype<Widget>("Widget", sol::no_constructor);

  table.new_usertype<Text>("Text", sol::no_constructor, "done", &Text::done,
                           "skip", &Text::skip, sol::base_classes,
                           sol::bases<Widget>());

  table.new_usertype<Window>("Window", sol::no_constructor, sol::base_classes,
                             sol::bases<Widget>());

  table["widget"] = [this](EntityId id) { return widgets[id].get(); };
  table["text_widget"] = [this](EntityId id) {
    return (Text *)widgets[id].get();
  };
}
} // namespace ecs

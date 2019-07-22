#include <memory>

#include <sol/sol.hpp>

#include "common/logging.hpp"

#include "ui/text.hpp"
#include "ui/ui.hpp"
#include "ui/window.hpp"

#include "framework/ui.hpp"

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
  auto context = ui::UiContext{batch, fonts};
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
} // namespace ecs

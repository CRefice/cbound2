#include "ui/ui.hpp"

#include "ui-manager.hpp"

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

void UiManager::draw_all(const Scene& scene) {
  auto context = ui::UiContext{fonts.load("fonts/font.fnt"), batch};
  for (auto &[id, widget] : widgets) {
		auto mvmt = scene.find(id);
    widget->draw(mvmt->pos, context);
  }
  batch.flush();
}
} // namespace ecs

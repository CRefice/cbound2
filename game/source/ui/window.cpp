#include "core/render/sprite.hpp"
#include "core/render/tex-coords.hpp"

#include "window.hpp"

namespace ui {
using namespace ::render;
static TexFrame top_left = TexFrame(ssm::ivec2(0, 0), ssm::ivec2(8, 8));
static TexFrame top_mid = TexFrame(ssm::ivec2(8, 0), ssm::ivec2(16, 8));
static TexFrame top_right = TexFrame(ssm::ivec2(16, 0), ssm::ivec2(24, 8));
static TexFrame mid_left = TexFrame(ssm::ivec2(0, 8), ssm::ivec2(8, 16));
static TexFrame mid_mid = TexFrame(ssm::ivec2(8, 8), ssm::ivec2(16, 16));
static TexFrame mid_right = TexFrame(ssm::ivec2(16, 8), ssm::ivec2(24, 16));
static TexFrame bottom_left = TexFrame(ssm::ivec2(0, 16), ssm::ivec2(8, 24));
static TexFrame bottom_mid = TexFrame(ssm::ivec2(8, 16), ssm::ivec2(16, 24));
static TexFrame bottom_right = TexFrame(ssm::ivec2(16, 16), ssm::ivec2(24, 24));

ssm::vec2 fsize(const TexFrame& frame) {
  float w = frame.width();
  float h = frame.height();
  return ssm::vec2(w, h);
}

void Window::draw(ssm::vec2 pos, UiContext& ctx) const {
  std::string texture = "textures/gfx/gui-elements.png";

  auto size = this->size();
  auto bottom_left_rect = Rectangle<float>(pos, pos + fsize(bottom_left));
  auto bl_corner = pos + ssm::vec2(size.x - bottom_right.width(), 0);
  auto bottom_right_rect =
      Rectangle<float>(bl_corner, bl_corner + fsize(bottom_right));
  auto tl_corner = pos + ssm::vec2(0, size.y - top_left.height());
  auto top_left_rect = Rectangle<float>(tl_corner, tl_corner + fsize(top_left));
  auto tr_corner = pos + size - fsize(top_right);
  auto top_right_rect =
      Rectangle<float>(tr_corner, tr_corner + fsize(top_right));

  auto bottom_mid_rect = Rectangle<float>(bottom_left_rect.bottom_right(),
                                          bottom_right_rect.top_left());
  auto top_mid_rect =
      Rectangle<float>(top_left_rect.bottom_right(), top_right_rect.top_left());
  auto mid_left_rect = Rectangle<float>(bottom_left_rect.top_left(),
                                        top_left_rect.bottom_right());
  auto mid_right_rect = Rectangle<float>(bottom_right_rect.top_left(),
                                         top_right_rect.bottom_right());
  auto mid_mid_rect = Rectangle<float>(bottom_left_rect.top_right(),
                                       top_right_rect.bottom_left());

  auto& batch = ctx.sprite_batch;
  batch.draw(Sprite{texture, top_left_rect.size(), top_left, 0.1f},
             top_left_rect.bottom_left());
  batch.draw(Sprite{texture, top_mid_rect.size(), top_mid, 0.1f},
             top_mid_rect.bottom_left());
  batch.draw(Sprite{texture, top_right_rect.size(), top_right, 0.1f},
             top_right_rect.bottom_left());
  batch.draw(Sprite{texture, mid_left_rect.size(), mid_left, 0.1f},
             mid_left_rect.bottom_left());
  batch.draw(Sprite{texture, mid_mid_rect.size(), mid_mid, 0.1f},
             mid_mid_rect.bottom_left());
  batch.draw(Sprite{texture, mid_right_rect.size(), mid_right, 0.1f},
             mid_right_rect.bottom_left());
  batch.draw(Sprite{texture, bottom_left_rect.size(), bottom_left, 0.1f},
             bottom_left_rect.bottom_left());
  batch.draw(Sprite{texture, bottom_mid_rect.size(), bottom_mid, 0.1f},
             bottom_mid_rect.bottom_left());
  batch.draw(Sprite{texture, bottom_right_rect.size(), bottom_right, 0.1f},
             bottom_right_rect.bottom_left());
}

std::unique_ptr<Widget> Window::clone() const {
  return std::make_unique<Window>(*this);
}
} // namespace ui

#include <vector>

#include <imgui/imgui.h>
#include <sol/sol.hpp>

#include "common/paths.hpp"
#include "core/script/script.hpp"

#include "io.hpp"
#include "tiles.hpp"

namespace editor {
using namespace ::render;
namespace fs = std::filesystem;

static void change_resolution(TileSet& tileset, ssm::ivec2 res) {
  tileset.collision.subtile_res = res;
  tileset.collision.data = Matrix<bool>(tileset.size * res);
}

static void draw_collisions(TileSet& tileset, const ImVec4& color,
                            ssm::vec2 tile_size, ImVec2 origin) {
  auto tile_dims = tileset.size * tileset.collision.subtile_res;
  for (int x = 0; x < tile_dims.x; ++x) {
    for (int y = 0; y < tile_dims.y; ++y) {
      if (tileset.collision.data[ssm::ivec2(x, y)]) {
        auto tl =
            ImVec2(origin.x + tile_size.x * x, origin.y + tile_size.y * y);
        auto br = tl;
        br.x += tile_size.x;
        br.y += tile_size.y;
        auto col = ImGui::ColorConvertFloat4ToU32(color);
        ImGui::GetWindowDrawList()->AddRectFilled(tl, br, col);
      }
    }
  }
}

static void handle_mouse(TileSet& tileset, ssm::vec2 tile_size, ImVec2 origin) {
  auto& io = ImGui::GetIO();
  io.ConfigWindowsMoveFromTitleBarOnly = true;
  if (ImGui::IsItemHovered() && (io.MouseDown[0] || io.MouseDown[1])) {
    bool pressed = io.MouseDown[0] ? 1 : 0;
    auto mouse = ImGui::GetMousePos();
    auto mouse_rel = ssm::vec2(mouse.x - origin.x, mouse.y - origin.y);
    auto tile_coords = ssm::ivec2(mouse_rel / tile_size);
    tileset.collision.data[tile_coords] = pressed;
  }
}

static void draw_grid(Texture& image, const ImVec4& color, TileSet& tileset,
                      float zoom) {
  ImGui::BeginChild("Image", ImVec2(-200, 0), false,
                    ImGuiWindowFlags_HorizontalScrollbar);

  auto size = ssm::ivec2(image.size().x * zoom, image.size().y * zoom);
  auto tile_dims = tileset.size * tileset.collision.subtile_res;
  auto tile_size = ssm::vec2(size) / ssm::vec2(tile_dims);

  auto top_left = ImGui::GetCursorScreenPos();
  auto bottom_right = ImVec2(top_left.x + image.size().x * zoom,
                             top_left.y + image.size().y * zoom);

  ImGui::Image((void*)(intptr_t)image.handle(), ImVec2(size.x, size.y));

  auto begin = top_left;
  auto end = ImVec2(top_left.x, bottom_right.y);
  for (int i = 0; i < tileset.size.x; ++i) {
    begin.x = top_left.x + tile_size.x * tileset.collision.subtile_res.x * i;
    end.x = begin.x;
    ImGui::GetWindowDrawList()->AddLine(begin, end,
                                        IM_COL32(180, 180, 180, 255), 1.5f);

    for (int j = 0; j < tileset.collision.subtile_res.x; ++j) {
      begin.x += tile_size.x;
      end.x += tile_size.x;
      ImGui::GetWindowDrawList()->AddLine(begin, end,
                                          IM_COL32(120, 120, 120, 255), 1.0f);
    }
  }

  begin = top_left;
  end = ImVec2(bottom_right.x, top_left.y);
  for (int i = 0; i < tileset.size.y; ++i) {
    begin.y = top_left.y + tile_size.y * tileset.collision.subtile_res.y * i;
    end.y = begin.y;
    ImGui::GetWindowDrawList()->AddLine(begin, end,
                                        IM_COL32(180, 180, 180, 255), 2.0f);

    for (int j = 0; j < tileset.collision.subtile_res.x; ++j) {
      begin.y += tile_size.y;
      end.y += tile_size.y;
      ImGui::GetWindowDrawList()->AddLine(begin, end,
                                          IM_COL32(120, 120, 120, 255), 1.0f);
    }
  }

  draw_collisions(tileset, color, tile_size, top_left);
  handle_mouse(tileset, tile_size, top_left);

  ImGui::EndChild();
}

static fs::path file_chooser() {
  ImGui::BeginChild("File Chooser", ImVec2(150, 0), true);
  static fs::path ret;
  auto res_path = path::install_dir() / "resources";
  for (const fs::path& p : fs::recursive_directory_iterator(res_path)) {
    if (p.extension() == ".tsx") {
      if (ImGui::Selectable(p.filename().c_str())) {
        ret = p;
      }
    }
  }
  ImGui::EndChild();
  return ret;
}

void tileset_editor(ResourceCache<::render::TileSet>& tilesets,
                    ResourceCache<::render::Texture>& textures) {
  ImGui::Begin("Tileset Editor");

  auto path = file_chooser();
  auto tileset = tilesets.load(path);

  static float zoom = 1.0f;
  static bool lock_dims = true;
  static ImVec4 color = ImVec4(0.1f, 0.1f, 0.7f, 0.5f);

  auto image = textures.load(tileset->texture_id);
  auto size = ssm::ivec2(image->size().x * zoom, image->size().y * zoom);
  ImGui::SameLine();
  ImGui::SetNextWindowContentWidth(size.x);
  draw_grid(*image, color, *tileset, zoom);

  ImGui::SameLine();
  ImGui::BeginChild("Controls");
  ImGui::SliderFloat("Zoom", &zoom, 1.0f, 10.0f);
  ImGui::Separator();
  ImGui::Text("Subtile Resolution");
  ImGui::Checkbox("Lock dimensions", &lock_dims);
  if (lock_dims) {
    int val = tileset->collision.subtile_res.x;
    if (ImGui::SliderInt("##subtilepicker", &val, 1, 10)) {
      change_resolution(*tileset, ssm::ivec2(val));
    }
  } else if (ImGui::SliderInt2("##subtilepicker",
                               ssm::data_ptr(tileset->collision.subtile_res), 1,
                               10)) {
    change_resolution(*tileset, tileset->collision.subtile_res);
  }
  ImGui::ColorEdit4("Collision GUI color", &color.x);
  if (ImGui::Button("Save")) {
    io::save_collision(tileset.identifier(), *tileset);
  }
  ImGui::EndChild();

  ImGui::End();
}
} // namespace editor

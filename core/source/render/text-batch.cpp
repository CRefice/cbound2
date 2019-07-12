#include <cctype>

#include "common/logging.hpp"
#include "core/script/script.hpp"

#include "render/text-batch.hpp"

namespace render {
std::optional<CharMetrics> metrics_of(std::string_view str,
                                      const TextDrawParams& params) {
  const auto& font = params.font;
	if (!font) return std::nullopt;
  const auto it = font->char_map.find(std::string(str));
  if (it == font->char_map.end()) {
    WARN_LOG("Couldn't find a glyph for character {} in font {}", str,
             font->texture_id);
    return std::nullopt;
  }
  return std::optional(it->second);
}

std::string split_whitespace(std::string_view str,
                             const TextDrawParams& params) {
  if (!params.max_width.has_value()) {
    return std::string(str);
  }

  std::string ret;
  std::size_t start = 0, end = 0;
  float width = 0.0f;
  for (char c : str) {
    if (auto metrics = metrics_of(std::string_view(&c, 1), params)) {
      width += metrics->frame.width() + params.char_spacing;
    }
    if (width >= *params.max_width) {
      ret += '\n';
      width = 0.0f;
    }
    end++;
    if (std::isspace(c)) {
      auto word = str.substr(start, end - start);
      ret += std::string(word);
      start = end;
    }
  }
  auto word = str.substr(start, end - start);
  ret += std::string(word);
  return ret;
}

void TextBatch::draw(std::string_view text, const ssm::vec2& pos, float layer,
                     ssm::vec4 color) {
  auto cursor = pos;
  auto newline = [&]() {
    cursor = ssm::vec2(pos.x, cursor.y - params.line_spacing);
  };

  for (auto str = text; !str.empty(); str.remove_prefix(1)) {
    char c = str[0];
    if (c == '\n') {
      newline();
      continue;
    }
    if (auto index = str.length();
        auto ctrl = text::parse_ctrl_code(str, &index)) {
      control_code(*ctrl, color);
      str.remove_prefix(index - 1);
      continue;
    }
    if (auto metrics = metrics_of(std::string_view(&c, 1), params)) {
      draw_glyph(*metrics, cursor, layer, color);
      cursor += ssm::vec2(metrics->frame.width() + params.char_spacing, 0);
    }
  }
}

void TextBatch::draw_glyph(const CharMetrics& cm, const ssm::vec2& pos,
                           float layer, const ssm::vec4& clr) {
  ssm::vec2 size(cm.frame.width(), cm.frame.height());
  Sprite sprite = {params.font->texture_id, size, cm.frame, layer, clr};
  batch.draw(sprite, pos + ssm::vec2(0, cm.vert_offset));
}

void TextBatch::control_code(const text::ControlCode& code,
                             ssm::vec4& out_color) {
  if (code.name == "color") {
    auto maybe_color = code.argument
                           ? script::parse_color(code.argument.value())
                           : std::nullopt;
    out_color = maybe_color.value_or(params.color);
  }
}
} // namespace render

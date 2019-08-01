#include "common/rectangle.hpp"

#include "render/post-process.hpp"

#include <GLFW/glfw3.h>

namespace render {
// Initialize the screen vao and vbo.
PostProcessStack::PostProcessStack(render::GLContext context) : context(context) {
  glBindVertexArray(screen_vao);
  glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);

  Rectangle<float> pos_rect(ssm::vec2(-1, -1), ssm::vec2(1, 1));
  Rectangle<float> uv_rect(ssm::vec2(0, 0), ssm::vec2(1, 1));

  // Vertex data format:
  // 0: pos.x pos.y
  // 1: uv.x uv.y
  ssm::vec2 data[2 * 6] = {
      // Triangle 1
      pos_rect.bottom_left(), uv_rect.bottom_left(), pos_rect.bottom_right(),
      uv_rect.bottom_right(), pos_rect.top_left(), uv_rect.top_left(),
      // Triangle 2
      pos_rect.top_left(), uv_rect.top_left(), pos_rect.bottom_right(),
      uv_rect.bottom_right(), pos_rect.top_right(), uv_rect.top_right()};

  glBufferData(GL_ARRAY_BUFFER, sizeof(data), ssm::data_ptr(data[0]),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(ssm::vec2),
                        reinterpret_cast<void*>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(ssm::vec2),
                        reinterpret_cast<void*>(sizeof(ssm::vec2)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void PostProcessStack::new_frame() {
  GLuint handle;
  ssm::ivec2 size;
  if (stack.empty()) {
    handle = 0;
    size = screen_size();
  } else {
    auto& fb = stack.front().frame_buffer;
    handle = fb.handle();
    size = fb.size();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, handle);
  glViewport(0, 0, size.x, size.y);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// The current fb texture will already be filled up.
// So use that, with the current program shader,
// to draw to the NEXT fb.
void PostProcessStack::draw_all() {
  if (stack.empty())
    return;

  glBindVertexArray(screen_vao);
  for (auto it = stack.begin(); it != stack.end(); ++it) {
    GLuint handle;
    ssm::ivec2 size;
    if (auto next = std::next(it); next != stack.end()) {
      auto& fb = next->frame_buffer;
      handle = fb.handle();
      size = fb.size();
    } else {
      handle = 0;
      size = screen_size();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
    glViewport(0, 0, size.x, size.y);
    glUseProgram(it->program.handle());
    glBindTexture(GL_TEXTURE_2D, it->texture.handle());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}


ssm::ivec2 PostProcessStack::screen_size() const {
  int width, height;
  glfwGetFramebufferSize(context, &width, &height);
	return ssm::ivec2(width, height);
}
} // namespace render

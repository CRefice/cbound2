#pragma once

#include <vector>

#include "glinterface/common.hpp"

#include "framebuffer.hpp"
#include "render.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace render {
// A post-process effect, with a texture
// that will contain its framebuffer data,
// and a shader to actually draw the postprocess with.
struct PostProcess {
  PostProcess(Texture&& tex, shader::Program&& prog)
      : texture(std::move(tex)), frame_buffer(texture),
        program(std::move(prog)) {}

  // The texture containing the color data of the framebuffer.
  Texture texture;
  // The framebuffer that this effect will be drawn on
  FrameBuffer frame_buffer;
  // The shader used to draw this effect on to the next framebuffer.
  shader::Program program;
};

// A stack of post-processing effects that will be
// piled on top of each other to draw images.
class PostProcessStack {
public:
  PostProcessStack(render::GLContext context);

  // Bind the first framebuffer on the stack
  // as the one to be drawn on, and clear it.
  // If the stack is empty, the default fb
  // will be used instead.
  void new_frame();

  // Walk through the stack and draw all
  // postprocess effects one after the other.
  void draw_all();

  // Add an effect to the stack
  template <typename... Args>
  void emplace_pass(Args&&... args) {
    stack.emplace_back(std::forward<Args>(args)...);
  }

  // Remove all effects from the stack
  void clear() { stack.clear(); }

  auto size() const { return stack.size(); }

private:
  ssm::ivec2 screen_size() const;

	render::GLContext context;
  gl::VertexArrayObject screen_vao;
  gl::BufferObject screen_vbo;
  std::vector<PostProcess> stack;
};
} // namespace render

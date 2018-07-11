#pragma once

#include <Gooey\Include\OpenGL\GLWindow.hpp>

#include "core\resource\resource.hpp"
#include "core\resource\shader.hpp"

namespace rendercommon
{
void init(Goo::GLWindow& window);
void shutdown();

void set_postprocess_shader(Resource<Shader> shader);
void set_viewport(vec2<uint> pos, vec2<uint> size);

void clear_screen();

void begin_draw();
void end_draw();
}

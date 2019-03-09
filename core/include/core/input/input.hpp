#pragma once

#include "core/render/render.hpp"

namespace input {
class KeyHandler;

void init(render::Context context);

void set_handler(render::Context context, KeyHandler& handler);
KeyHandler& get_handler(render::Context context);

void shutdown(render::Context context);
} // namespace input

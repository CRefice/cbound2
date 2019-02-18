#pragma once

#include "core/render/render.hpp"

class ContextStack;

namespace input {
void init(render::Context context);
ContextStack& get_context(render::Context context);
void shutdown(render::Context context);
}

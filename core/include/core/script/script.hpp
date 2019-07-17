#pragma once

#include <optional>

#include <sol/sol.hpp>
#include <ssm/vector.hpp>

namespace script {
std::optional<ssm::vec4> parse_color(std::string_view str);

// Create a new environment
// that a singular script can be sourced in.
sol::state new_environment();

// Define common types and functions in the given state.
void load_common_libs(sol::state_view state);

sol::protected_function_result on_error(lua_State*,
                                        sol::protected_function_result pfr);
} // namespace script

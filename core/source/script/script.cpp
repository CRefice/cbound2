#include "script/script.hpp"

namespace script {
sol::state new_environment() {
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package);
	return lua;
}
}

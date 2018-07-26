#include "common/paths.hpp"

#ifdef _WIN32
#	include <Windows.h>
#elif defined __linux__
# include <climits>
# include <cstdlib>
# include <unistd.h>
#endif

namespace path
{
fs::path executable_path() {
	static fs::path exe_dir;
	if (exe_dir.empty()) {
#ifdef _WIN32
	TCHAR exe_name[MAX_PATH];
	::GetModuleFileName(nullptr, exe_name, MAX_PATH);
	exe_dir = exe_name;
#elif defined __linux__
	char* exe_name = realpath("/proc/self/exe", nullptr);
	exe_dir = exe_name;
	std::free(exe_name);
#else
#error "executable_path() is not implemented for this platform"
#endif
	}
	return exe_dir;
}

fs::path install_dir() {
	return executable_path().parent_path();
}
}

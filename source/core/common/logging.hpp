#pragma once

#include <fmt/ostream.h>
#include <fmt/time.h>

#include <fstream>
#include <memory>
#include <string_view>

#ifndef LOG_FILE_PATH
# include "paths.hpp"
#	define LOG_FILE_PATH (path::install_dir() / "exec.log")
#endif

namespace log
{
enum LogLevel {
	Debug, Info, Warn, Error
};
std::string_view to_string(LogLevel level) {
	switch (level) {
		case LogLevel::Debug: return "DEBUG";
		case LogLevel::Info: return "INFO";
		case LogLevel::Warn: return "WARNING";
		case LogLevel::Error: return "ERROR";
	}
}

// Takes care of formatting the given inputs,
// and passes them along to the sink.
// Sinks must provide a write(std::string_view) method.
template <typename Sink>
class Logger
{
public:
	Logger(Sink& sink) : sink(sink) {}

	// Note that input must be in fmt format strings.
	template<typename... Args>
	void log(LogLevel level, Args... args) {
		if (level < min_level) return;
		auto t = std::time(nullptr);
		sink.write(fmt::format("{%H:%M:%S} ~ [{}]: ",
					std::localtime(&t), to_string(level)));
		sink.write(fmt::format(args...));
	}

	// Sets the minimum log level for this particular log.
	// Any successive log of a lower level
	// than the provided one will be skipped.
	void filter(LogLevel minimum) {
		min_level = minimum;
	}

private:
	LogLevel min_level = 0;
	Sink& out;
};

// Writes logs down to a file.
class FileSink
{
public:
	FileSink(std::ofstream os) : out(std::move(os)) {}

	void write(std::string_view line) {
		out << str << '\n';
	}

private:
	std::ofstream out;
};

// The common log used by macros.
// Wrapped in a function to avoid being created in release builds.
inline auto shared_log() {
	static FileSink sink(std::ofstream(LOG_FILE_PATH, std::ios::trunc));
	static Logger log(sink);
	return log;
}
}

#ifdef _DEBUG || defined(DEBUGFAST)
#	define DEBUG_LOG(...) do { log::shared_log().log(Debug, ...); } while(0)
#	define INFO_LOG(...) do { log::shared_log().log(Info, ...); } while(0)
#	define WARN_LOG(...) do { log::shared_log().log(Warn, ...); } while(0)
#	define ERROR_LOG(...) do { log::shared_log().log(Error, ...); } while(0)
#else
#	define DEBUG_LOG(...)
#	define INFO_LOG(...)
#	define WARN_LOG(...)
#	define ERROR_LOG(...)
#endif

#pragma once
#include <fmt/core.h>
#include <fmt/time.h>
// Since fmt defines the "fmt" macro, which fucks up other stuff,
// we undefine it. YAGNI, anyways.
#undef fmt

#include <fstream>
#include <memory>
#include <string_view>

#ifndef LOG_FILE_PATH
#include "paths.hpp"
#define LOG_FILE_PATH (path::install_dir() / "exec.log")
#endif

namespace logging {
enum LogLevel { Debug, Info, Warn, Error, Fatal };
inline std::string_view to_string(LogLevel level) {
  switch (level) {
  case LogLevel::Debug:
    return "DEBUG";
  case LogLevel::Info:
    return "INFO";
  case LogLevel::Warn:
    return "WARNING";
  case LogLevel::Error:
    return "ERROR";
  case LogLevel::Fatal:
    return "FATAL";
  default:
    return "UNKNOWN";
  }
}

// Takes care of formatting the given inputs,
// and passes them along to the sink.
// Sinks must provide a write(std::string_view) method.
template <typename Sink> class Logger {
public:
  Logger(Sink &sink) : out(sink) {}

  // Note that input must be in fmt format strings.
  template <typename... Args> void log(LogLevel level, Args... args) {
    if (level < min_level)
      return;
    auto t = std::time(nullptr);
    out.write(fmt::format("{:%H:%M:%S} ~ [{}]: ", *std::localtime(&t),
                          to_string(level)));
    out.writeln(fmt::format(args...));
  }

  // Sets the minimum log level for this particular log.
  // Any successive log of a lower level
  // than the provided one will be skipped.
  void filter(LogLevel minimum) { min_level = minimum; }

private:
  LogLevel min_level = Debug;
  Sink &out;
};

// Writes debug messages to a log on construction and destruction.
// Extremely useful for function debugging.
template <typename Sink, typename... Args>
class ScopedLogger
{
public:
	ScopedLogger(Logger<Sink>& log, Args... args) : log(log),
	title(fmt::format(args...)) {
		log.log(LogLevel::Debug, "Entering: {}", title);
	}

	~ScopedLogger() {
		log.log(LogLevel::Debug, "Exited: {}", title);
	}

private:
	Logger<Sink>& log;
	std::string title;
};

// Writes logs down to a file.
class FileSink {
public:
  FileSink(std::ofstream os) : out(std::move(os)) {}

  void write(std::string_view line) { out << line; }
  void writeln(std::string_view line) { out << line << std::endl; }

private:
  std::ofstream out;
};

// The common log used by macros.
// Wrapped in a function to avoid being created in release builds.
inline auto& shared_log() {
  static FileSink sink(std::ofstream(LOG_FILE_PATH, std::ios::trunc));
  static Logger log(sink);
  return log;
}

} // namespace logging

#ifndef NDEBUG
#define DEBUG_LOG(...)                                                         \
  do {                                                                         \
    logging::shared_log().log(logging::Debug, __VA_ARGS__);                    \
  } while (0)
#define INFO_LOG(...)                                                          \
  do {                                                                         \
    logging::shared_log().log(logging::Info, __VA_ARGS__);                     \
  } while (0)
#define WARN_LOG(...)                                                          \
  do {                                                                         \
    logging::shared_log().log(logging::Warn, __VA_ARGS__);                     \
  } while (0)
#define ERROR_LOG(...)                                                         \
  do {                                                                         \
    logging::shared_log().log(logging::Error, __VA_ARGS__);                    \
  } while (0)
#define FATAL_LOG(...)                                                         \
  do {                                                                         \
    logging::shared_log().log(logging::Fatal, __VA_ARGS__);                    \
  } while (0)

#define SCOPE_LOG(...) logging::ScopedLogger _scope_log_{logging::shared_log(), __VA_ARGS__}
#define FN_LOG logging::ScopedLogger _fn_log_(logging::shared_log(), "function {}", __func__)
#else
#define DEBUG_LOG(...)
#define INFO_LOG(...)
#define WARN_LOG(...)
#define ERROR_LOG(...)
#define FATAL_LOG(...)

#define SCOPE_LOG(...)
#define FN_LOG
#endif

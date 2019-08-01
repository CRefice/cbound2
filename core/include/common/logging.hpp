#pragma once
#include <fmt/core.h>
// Since fmt defines the "fmt" macro, which fucks up other stuff,
// we undefine it. YAGNI, anyways.
#undef fmt

#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>
#include <tuple>

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
template <typename Sink>
class Logger {
public:
  // Any successive log of a lower level
  // than the minimum will be skipped.
  Logger(Sink& sink, LogLevel minimum = LogLevel::Debug)
      : out(sink), min_level(minimum) {}

  // Note that input must be in fmt format strings.
  template <typename... Args>
  void log(LogLevel level, Args&&... args) {
    if (level < min_level)
      return;
    out.write(fmt::format("[{}]: ", to_string(level)));
    out.writeln(fmt::format(std::forward<Args>(args)...));
  }

private:
  Sink& out;
  LogLevel min_level = Debug;
};

// Writes logs to multiple loggers at once, each with its own
// minimum level.
template <typename... Loggers>
class MultiLogger {
public:
  MultiLogger(Loggers&&... loggers) : logs(std::forward<Loggers>(loggers)...) {}

  template <typename... Args>
  void log(LogLevel level, Args&&... args) {
    std::apply(
        [&](auto&... x) { (..., x.log(level, std::forward<Args>(args)...)); },
        logs);
  }

private:
  std::tuple<Loggers...> logs;
};

// Writes debug messages to a log on construction and destruction.
// Extremely useful for function debugging.
template <typename Log, typename... Args>
class ScopedLogger {
public:
  ScopedLogger(Log& log, Args... args) : log(log), title(fmt::format(args...)) {
    log.log(LogLevel::Debug, "Entering: {}", title);
  }

  ~ScopedLogger() { log.log(LogLevel::Debug, "Exited: {}", title); }

private:
  Log& log;
  std::string title;
};

// Writes logs to a std::stream
class StreamSink {
public:
  StreamSink(std::ostream& os) : out(os) {}

  void write(std::string_view line) { out << line; }
  void writeln(std::string_view line) { out << line << std::endl; }

private:
  std::ostream& out;
};

// The common log used by macros.
// Wrapped in a function to avoid being created in release builds.
inline auto& shared_log() {
  static std::ofstream file_stream{LOG_FILE_PATH, std::ios::trunc};
  static StreamSink file_sink(file_stream);
  static StreamSink cerr_sink(std::cerr);
  // Write all outputs to file, only critical to stderr
  static MultiLogger log{Logger(file_sink), Logger(cerr_sink, LogLevel::Fatal)};
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

#define SCOPE_LOG(...)                                                         \
  logging::ScopedLogger _scope_log_ { logging::shared_log(), __VA_ARGS__ }
#define FN_LOG                                                                 \
  logging::ScopedLogger _fn_log_(logging::shared_log(), "function {}", __func__)
#else
#define DEBUG_LOG(...)
#define INFO_LOG(...)
#define WARN_LOG(...)
#define ERROR_LOG(...)
#define FATAL_LOG(...)

#define SCOPE_LOG(...)
#define FN_LOG
#endif

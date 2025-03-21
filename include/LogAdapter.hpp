#pragma once
#include "LumosLog.hpp"
#include <spdlog/fmt/ostr.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/spdlog.h>

void spdlogAdapter(LumosLog::LogLevel level, const std::string& module, const std::string& msg) {
  std::string logMsg = "[" + module + "] " + msg;
  switch (level) {
    case LumosLog::LogLevel::TRACE:
      spdlog::trace(logMsg);
      break;
    case LumosLog::LogLevel::DEBUG:
      spdlog::debug(logMsg);
      break;
    case LumosLog::LogLevel::INFO:
      spdlog::info(logMsg);
      break;
    case LumosLog::LogLevel::WARN:
      spdlog::warn(logMsg);
      break;
    case LumosLog::LogLevel::ERROR:
      spdlog::error(logMsg);
      break;
    case LumosLog::LogLevel::FATAL:
      spdlog::critical(logMsg);
      break;
  }
}

void initLumosLog() {
  spdlog::stdout_color_mt("http_framework");
  spdlog::set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] [PID:%P] [TID:%t] %v");
  spdlog::set_level(spdlog::level::debug);
  LumosLog::LumosLog::setLogger(spdlogAdapter);

  LumosLog::log(LumosLog::LogLevel::INFO, "init", "init LumosLog");
}
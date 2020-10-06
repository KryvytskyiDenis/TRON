#pragma once

enum LogLevel
{
    TRACE,
    INFO,
    WARN,
    ERROR,
    CRITICAL
};

class Log
{
public:
    static void log(LogLevel level, const char* message);
};

#define TRACE(msg) ::Log::log(LogLevel::TRACE, msg)
#define INFO(msg) ::Log::log(LogLevel::INFO, msg)
#define WARN(msg) ::Log::log(LogLevel::WARN, msg)
#define ERROR(msg) ::Log::log(LogLevel::ERROR, msg)
#define CRITICAL(msg) ::Log::log(LogLevel::CRITICAL, msg)
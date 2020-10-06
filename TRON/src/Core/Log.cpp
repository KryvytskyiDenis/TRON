#include "trpch.h"
#include "Core/Log.h"

static const char* level_string_views[]
{
    "Trace", "Info", "Warn", "Error", "Critical"
};

void Log::log(LogLevel level, const char* message)
{
    std::cout << level_string_views[level] << ": " << message << std::endl;
}

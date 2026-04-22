#include "Log.h"

#include <iostream>
#include <filesystem>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "OpenGLMinecraft/Debug.h"

#define RELEASE_LOG_LEVEL spdlog::level::trace
std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init(
    const spdlog::level::level_enum p_LoggerLevel,
    const spdlog::level::level_enum p_ConsoleLevel,
    const spdlog::level::level_enum p_FileLevel,
    const std::filesystem::path& p_LogFile)
{
    spdlog::set_pattern("%^[%T] %n: %v%$");

    try
    {
        // No point making a console logger for release builds, potentially up FileSink log level in release as well
        #ifdef NDEBUG
        auto FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(p_LogFile.string(), true);
        FileSink->set_level(RELEASE_LOG_LEVEL);
        #else
        auto ConsoleSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        ConsoleSink->set_level(p_ConsoleLevel);
        auto FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(p_LogFile.string(), true);
        FileSink->set_level(p_FileLevel);
        #endif

        spdlog::sinks_init_list sinks = {FileSink
        #ifndef NDEBUG 
            ,ConsoleSink
        #endif
        };

        s_Logger = std::make_shared<spdlog::logger>("OpenGLMinecraft", sinks.begin(), sinks.end());
        s_Logger->set_level(p_LoggerLevel);
    }
    catch(const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << "\n";
        ASSERT(false);
    }
}

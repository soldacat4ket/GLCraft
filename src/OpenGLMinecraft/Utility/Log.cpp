#include "Log.h"

#include <iostream>
#include <filesystem>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "OpenGLMinecraft/Debug.h"

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
        auto ConsoleSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        ConsoleSink->set_level(p_ConsoleLevel);
        auto FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(p_LogFile.string(), true);
        FileSink->set_level(p_FileLevel);

        spdlog::sinks_init_list sinks = {ConsoleSink, FileSink};
        s_Logger = std::make_shared<spdlog::logger>("OpenGLMinecraft", sinks.begin(), sinks.end());
        s_Logger->set_level(p_LoggerLevel);
    }
    catch(const spdlog::spdlog_ex& ex)
    {
        std::cerr << "You really fucked up now. Heres why you're stupid ->" << ex.what() << "\n";
        ASSERT(false);
    }
}

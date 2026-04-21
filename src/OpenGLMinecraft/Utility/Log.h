#pragma once

#include <memory>
#include <filesystem>

#define SPDLOG_USE_STD_FORMAT
#include <spdlog/spdlog.h>

#include "OpenGLMinecraft/Utility/Singleton.h"

class Log : Singleton
{
public:
    static void Init(
        spdlog::level::level_enum p_LoggerLevel = spdlog::level::trace,
        spdlog::level::level_enum p_ConsoleLevel = spdlog::level::debug, 
        spdlog::level::level_enum p_FileLevel = spdlog::level::trace, 
        const std::filesystem::path& p_LogFile = std::filesystem::current_path() / "res" / "logs"/ "log.txt");

    inline static std::shared_ptr<spdlog::logger>& Get() { return s_Logger; }
private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

//LOG MACROS
#define LOG_TRACE(...)    ::Log::Get()->trace(__VA_ARGS__) // for the little things
#define LOG_DEBUG(...)    ::Log::Get()->debug(__VA_ARGS__) // for specific debug info
#define LOG_INFO(...)     ::Log::Get()->info(__VA_ARGS__) // for application control info
#define LOG_WARN(...)     ::Log::Get()->warn(__VA_ARGS__) // you better watch it mister
#define LOG_ERROR(...)    ::Log::Get()->error(__VA_ARGS__) // wtf
#define LOG_FATAL(...)    ::Log::Get()->critical(__VA_ARGS__) // gg early extraction
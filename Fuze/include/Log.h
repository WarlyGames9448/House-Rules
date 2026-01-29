#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>

namespace Fuze {
class FUZE_API Log {
  public:
    static void init();
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_Core_Logger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_Client_Logger; }

  private:
    static std::shared_ptr<spdlog::logger> s_Core_Logger;
    static std::shared_ptr<spdlog::logger> s_Client_Logger;
};
}

// Core Logger
#define FUZE_CORE_TRACE(...)         ::Fuze::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FUZE_CORE_INFO(...)          ::Fuze::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FUZE_CORE_WARN(...)          ::Fuze::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FUZE_CORE_ERROR(...)         ::Fuze::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FUZE_CORE_CRITICAL(...)      ::Fuze::Log::GetCoreLogger()->critical(__VA_ARGS__)F
// App Logger
#define FUZE_TRACE(...)         ::Fuze::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FUZE_INFO(...)          ::Fuze::Log::GetClientLogger()->info(__VA_ARGS__)
#define FUZE_WARN(...)          ::Fuze::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FUZE_ERROR(...)         ::Fuze::Log::GetClientLogger()->error(__VA_ARGS__)
#define FUZE_CRITICAL(...)      ::Fuze::Log::GetClientLogger()->critical(__VA_ARGS__)

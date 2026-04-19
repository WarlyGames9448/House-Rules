#include "fuzepch.h"
#include "Log.h"
#include "spdlog/common.h"

namespace Fuze {
std::shared_ptr<spdlog::logger> Log::s_Core_Logger;
std::shared_ptr<spdlog::logger> Log::s_Client_Logger;

void Log::init() {
    spdlog::set_pattern("%^[%T.%e] (%n) %v%$");
    s_Core_Logger = spdlog::stdout_color_mt("Fuze");
    s_Core_Logger->set_level(spdlog::level::trace);

    s_Client_Logger = spdlog::stdout_color_mt("APP");
    s_Client_Logger->set_level(spdlog::level::trace);
}
}

//
// Created by toor on 11/14/23.
//

#include "Logging.h"

namespace Atom {

    Ref<spdlog::logger> Log::s_Logger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_Logger = spdlog::stdout_color_mt("Bright");
        s_Logger->set_level(spdlog::level::trace);

    }

}
#pragma once

#if defined(_WIN32) || defined(_WIN64)
#endif

#if defined(__APPLE__) && defined(__MACH__)
#endif

#if defined(linux) || defined(__linux__) || defined(__linux)
#include "linux/system_monitor_factory.hpp"
#endif
#pragma once

#include <string>

/** @brief Represents individual CPU core load */
struct CpuLoad
{
    /** @brief CPU identifier */
    std::string cpu_id;
    /** @brief CPU usage percentage */
    double usage_percentage;
};

#pragma once

#include <string>

/** @brief Represents individual CPU core load */
struct CpuLoad
{
    /** @brief CPU identifier */
    std::string cpu_id;
    /** @brief CPU usage percentage */
    double usage_percentage;

    /** @brief Calc usage percent and return integer value. */
    size_t get_usage_percent_int() const { return usage_percentage * 100; }

    /** @brief Calc usage percent and return floating point value. */
    double get_usage_percent_float() const { return usage_percentage * 100; }
};

#pragma once

#include "system_info.hpp"

/** @brief System info printer */
struct IPrinter
{
    /**
     * @brief Prints system info.
     *
     * @param Struct holds system monitoring information.
     */
    virtual void print(SystemInfo&) = 0;

    virtual ~IPrinter() = default;
};
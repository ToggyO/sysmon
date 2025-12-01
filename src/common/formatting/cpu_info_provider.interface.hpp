#pragma once

#include "sys_info/cpu_load.hpp"

namespace sys_format
{
    /** @brief CPU stats provider. */
    struct ICpuInfoProvider
    {
        /** @brief Virtual dtor. */
        virtual ~ICpuInfoProvider() = default;

        /** @brief Get the number of cpu cores. */
        virtual size_t get_cpu_cores_count() const = 0;

        /** @brief Get cpu core brief info. */
        virtual const CpuLoad& get_cpu_core_info(size_t index) const = 0;
    };
} // namespace sys_format

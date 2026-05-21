#pragma once

#include <string>
#include <vector>

#include "./cpu_info_provider.interface.hpp"
#include "./memory_info_provider.interface.hpp"
#include "common/formatting/procceses_info_provider.interface.hpp"

namespace sys_format
{
    /** @brief Functionality for preparing system information for display. */
    struct ISystemInfoProvider
    {
        /** @brief Virtual dctor. */
        virtual ~ISystemInfoProvider() = default;

        /**
         * @brief Returns a reference to a RAM stats provider.
         *
         * @return Instance of @link IMemoryInfoProvider @endlink.
         */
        virtual const IMemoryInfoProvider& get_mem_info_provider() const = 0;

        /**
         * @brief Returns a reference to a CPU stats provider.
         *
         * @return Instance of @link ICpuInfoProvider @endlink.
         */
        virtual const ICpuInfoProvider& get_cpu_info_provider() const = 0;

        /**
         * @brief Returns a reference to a system processes stats provider.
         *
         * @return Instance of @link IProcessesInfoProvider @endlink.
         */
        virtual const IProcessesInfoProvider& get_processes_info_provider() const = 0;
    };
} // namespace sys_format

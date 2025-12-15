#pragma once

#include "../conversions.h" // BYTE_UNITS
#include <string>

namespace sys_format
{
    /** @brief Types of RAM stats values. */
    enum class RAMTotalOrUsed
    {
        TOTAL,
        USED
    };

    /** @brief Types of swap stats values. */
    enum class SwapTotalOrFree
    {
        TOTAL,
        FREE
    };

    /** @brief RAM stats provider. */
    struct IMemoryInfoProvider
    {
        /** @brief Virtual dctor. */
        virtual ~IMemoryInfoProvider() = default;

        /**
         * @brief Returns an amount of total or used RAM, converted into necessary format.
         *
         * @param mem_value_type type of RAM stats value.
         *
         * @param to units the value will be converted into.
         *
         * @return converted value.
         */
        virtual double get_converted_mem_value(RAMTotalOrUsed mem_value_type, BYTE_UNITS to) const = 0;

        /**
         * @brief Returns an amount of total or used swap, converted into necessary format.
         *
         * @param mem_value_type type of swap stats value.
         *
         * @param to units the value will be converted into.
         *
         * @return converted value.
         */
        /** @brief . */
        virtual double get_converted_swap_value(SwapTotalOrFree swap_value_type, BYTE_UNITS to) const = 0;

        /**
         * @brief Coverts target num into string.
         *
         * @param target Target num.
         *
         * @param precision Result value precision.
         */
        virtual std::string to_string(const double target, size_t precision) const = 0;
    };
} // namespace sys_format

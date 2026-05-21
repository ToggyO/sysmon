#pragma once

/** @brief Represents system RAM stats */
struct MemoryStats
{
    /** @brief Total memory count */
    double total_memory;
    /** @brief Memory count in use */
    double used_memory;
    /** @brief The amount of unused or available swap space */
    double swap_free;
    /** @brief The total amount of swap space */
    double swap_total;
};
#pragma once

/** @brief Represents CPU usage stats */
struct CpuStats
{
    /** @brief Average CPU usage in user mode */
    size_t user;
    /** @brief Average CPU usage in nice */
    size_t nice;
    /** @brief Average CPU usage in system mode */
    size_t system;
    /** @brief Average CPU usage in idle */
    size_t idle;
    /** @brief Average CPU usage in I/O wait */
    size_t iowait;
    /** @brief Average CPU usage in irq */
    size_t irq;
    /** @brief Average CPU usage in softirq */
    size_t softirq;
    /** @brief Average CPU usage in steal mode */
    size_t steal;
    /** @brief Average CPU usage in guest mode */
    size_t guest;
    /** @brief Average CPU usage in guest_nice */
    size_t guest_nice;

    // TODO :add descr
    size_t get_total_active() const
    {
        return user + nice + system + irq + softirq + steal + guest + guest_nice;
    }
    // TODO :add descr
    size_t get_total_idle() const { return idle + iowait; }
};
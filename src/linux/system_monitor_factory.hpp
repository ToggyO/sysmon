#pragma once

#include <memory>

#include "filesystem/system_files_reader.interface.hpp"
#include "filesystem/system_files_reader_linux.hpp"
#include "sys_info/printer.interface.hpp"
#include "system_monitor/system_monitor.hpp"

/** @brief Factory for SystemMonitor instances */
struct SystemMonitorFactory
{
    /** @brief Creates new instance of SystemMonitor and it's dependencies.
     *
     * @return instance of SystemMonitor
     */
    std::unique_ptr<SystemMonitor> create();
};

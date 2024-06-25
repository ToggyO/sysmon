#pragma once

#include <memory>

#include "system_monitor/system_monitor.hpp"
#include "filesystem/system_files_reader.interface.hpp"
#include "filesystem/system_files_reader_linux.hpp"

/** @brief Factory for SystemMonitor instances */
class SystemMonitorFactory
{
public:
    /** @brief Creates new instance of SystemMonitor and it's dependencies.
     *
     * @return instance of SystemMonitor
     */
    SystemMonitor create();

private:
    std::shared_ptr<ISystemFilesReader> m_reader_ptr;
};

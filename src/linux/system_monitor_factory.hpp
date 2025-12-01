#pragma once

#include <memory>

#include "filesystem/system_files_reader.interface.hpp"
#include "filesystem/system_files_reader_linux.hpp"
#include "sys_info/printer.interface.hpp"
#include "system_monitor/system_monitor.hpp"

/** @brief Factory for SystemMonitor instances */
class SystemMonitorFactory
{
public:
    /** @brief Creates new instance of SystemMonitor and it's dependencies.
     *
     * @return instance of SystemMonitor
     */
    std::unique_ptr<SystemMonitor> create();

private:
    //    std::shared_ptr<IFileDescriptorsCache> m_fd_cache_ptr; // TODO: check
    //    std::shared_ptr<ISystemFilesReader> m_reader_ptr;
    //    std::shared_ptr<IPrinter> m_printer_ptr;
};

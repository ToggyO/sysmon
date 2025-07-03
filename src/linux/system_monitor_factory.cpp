#include "system_monitor_factory.hpp"
#include "filesystem/file_descriptors_cache.hpp"
#include "../printer/ftxui_printer.hpp"

SystemMonitor SystemMonitorFactory::create()
{
    if (!m_fd_cache_ptr)
    {
        m_fd_cache_ptr = std::make_shared<FileDescriptorsCache>();
    }

    if (!m_reader_ptr)
    {
        m_reader_ptr = std::make_shared<SystemFilesReaderLinux>(m_fd_cache_ptr);
    }

    if (!m_printer_ptr)
    {
        m_printer_ptr = std::make_shared<FtxUiPrinter>();
    }

    return SystemMonitor(m_reader_ptr, m_printer_ptr);
}
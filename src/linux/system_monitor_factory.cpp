#include "system_monitor_factory.hpp"
#include "filesystem/file_descriptors_cache.hpp"

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

    return SystemMonitor(m_reader_ptr);
}
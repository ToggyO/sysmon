#include "system_monitor_factory.hpp"
#include "filesystem/file_descriptors_cache.hpp"

std::unique_ptr<SystemMonitor> SystemMonitorFactory::create()
{
    auto fd_cache_ptr = std::make_shared<FileDescriptorsCache>();
    auto reader_ptr = std::make_shared<SystemFilesReaderLinux>(std::move(fd_cache_ptr));
    return std::make_unique<SystemMonitor>(std::move(reader_ptr));
}

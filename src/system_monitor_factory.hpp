#pragma once

#if defined(_WIN32) || defined(_WIN64)
#endif

#if defined(__APPLE__) && defined(__MACH__)
#endif

#if defined(linux) || defined(__linux__) || defined(__linux)
// #include "linux/system_monitor/system_monitor.hpp"
// #include "linux/filesystem/system_files_reader_linux.hpp"
#include "linux-epoll/system_monitor.hpp"
#include "linux-epoll/linux_system_poller.hpp"
#include "linux-epoll/linux_system_file_descriptors_handler.hpp"
#endif

/** @brief Factory for SystemMonitor instances */
class SystemMonitorFactory
{
    public:
        /** @brief Creates new pointer of SystemMonitor instance and it's dependencies
         * @return Pointer to instance of SystemMonitor
         */
        SystemMonitor* create()
        {
#if defined(_WIN32) || defined(_WIN64)
    throw std::runtime_error("Unsupported OS!");
#endif

#if defined(__APPLE__) && defined(__MACH__)
    throw std::runtime_error("Unsupported OS!");
#endif

#if defined(linux) || defined(__linux__) || defined(__linux)
            // m_system_files_reader_linux = new SystemFilesReaderLinux();
            // return new SystemMonitor(m_system_files_reader_linux);
            std::shared_ptr<ISystemFileDescriptorsHandler> handler = std::make_shared<LinuxSystemFileDescriptorsHandler>();
            std::shared_ptr<IPoller> poller = std::make_shared<SystemPoller>(handler);
            return new SystemMonitor(poller);
#endif
        }

        // TODO: remove
        /** @brief Claer pointer to SystemMonitor instance and it's dependencies
         * @param Pointer to instance of SystemMonitor
        */
        void clear(SystemMonitor *system_monitor)
        {
#if defined(_WIN32) || defined(_WIN64)
            throw std::runtime_error("Unsupported OS!");
#endif

#if defined(__APPLE__) && defined(__MACH__)
            throw std::runtime_error("Unsupported OS!");
#endif

#if defined(linux) || defined(__linux__) || defined(__linux)
            // delete m_system_files_reader_linux;
            delete system_monitor;
#endif
        }

    private:
#if defined(_WIN32) || defined(_WIN64)
#endif

#if defined(__APPLE__) && defined(__MACH__)
#endif

#if defined(linux) || defined(__linux__) || defined(__linux)
        // SystemFilesReaderLinux *m_system_files_reader_linux;
#endif
};

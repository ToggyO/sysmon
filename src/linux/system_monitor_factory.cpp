#include "system_monitor_factory.hpp"

/** @brief
 *
 * @return
 */
SystemMonitor SystemMonitorFactory::create()
{
    if (!m_reader_ptr)
    {
        m_reader_ptr = std::make_shared<SystemFilesReaderLinux>();
    }
    return SystemMonitor(m_reader_ptr);
}
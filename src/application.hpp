#pragma once

#include <chrono>
#include <thread>

#include "sys_info/system_info.hpp"
#include "sysmon_headers.h"

#include "printer/ftxui_printer.hpp"
#include "provider/system_info_provider.hpp"

// TODO: add descr
class Application
{
public:
    Application();

    int run(const volatile sig_atomic_t& stop);

private:
    void await_for_new_iteration(const volatile sig_atomic_t& stop,
                                 const std::chrono::time_point<std::chrono::system_clock>& start_time,
                                 const std::chrono::milliseconds& iteration_duration,
                                 const std::chrono::milliseconds& quant_duration);

    SystemInfo m_system_info;
    std::shared_ptr<sys_format::ISystemInfoProvider> m_provider_ptr;
    std::unique_ptr<SystemMonitor> m_collector_ptr;
    std::unique_ptr<IPrinter> m_printer_ptr;
};

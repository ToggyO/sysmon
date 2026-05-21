#pragma once

#include <chrono>
#include <thread>

#include "sys_info/system_info.hpp"
#include "sysmon_headers.h"

#include "printer/ftxui_printer.hpp"
#include "provider/system_info_provider.hpp"

/**
 * @brief Coordinates system data collection and rendering.
 *
 * Owns the current system information snapshot, the platform-specific
 * collector, and the configured printer. The application object is not
 * copyable or movable because owned components may keep references to
 * @link Application::m_system_info @endlink.
 */
class Application
{
public:
    /** @brief Creates an application instance with default collector and printer implementations. */
    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    /** @brief Destroys the application and releases owned runtime components. */
    ~Application() = default;

    /**
     * @brief Starts the main monitoring loop.
     *
     * Collects system statistics, sends them to the printer, and waits between
     * refresh iterations until the stop flag is raised.
     *
     * @param stop Signal-aware flag that requests application shutdown when set.
     *
     * @return EXIT_SUCCESS on normal shutdown, EXIT_FAILURE when an exception is caught.
     */
    int run(const volatile sig_atomic_t& stop);

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

private:
    /**
     * @brief Waits until the next collection iteration should start.
     *
     * Periodically checks the stop flag while waiting, so shutdown requests can
     * interrupt the sleep interval.
     *
     * @param stop Signal-aware flag that requests application shutdown when set.
     * @param start_time Timestamp captured at the beginning of the current iteration.
     * @param iteration_duration Target duration of one collection iteration.
     * @param quant_duration Sleep quantum used between stop flag checks.
     */
    void await_for_new_iteration(const volatile sig_atomic_t& stop,
                                 const std::chrono::time_point<std::chrono::system_clock>& start_time,
                                 const std::chrono::milliseconds& iteration_duration,
                                 const std::chrono::milliseconds& quant_duration);

    /** @brief Current system information snapshot updated by the collector and rendered by the printer. */
    SystemInfo m_system_info;

    /** @brief Platform-specific system data collector owned by the application. */
    std::unique_ptr<SystemMonitor> m_collector_ptr;

    /** @brief System information printer owned by the application. */
    std::unique_ptr<IPrinter> m_printer_ptr;
};

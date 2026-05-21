#pragma once

#include <algorithm>
#include <array>                                  // std::array
#include <atomic>                                 // std::atomic_bool
#include <cmath>                                  // std::ceil
#include <csignal>                                // std::raise, SIGABRT
#include <ftxui/component/component.hpp>          // ftxui::Renderer
#include <ftxui/component/screen_interactive.hpp> // ftxui::ScreenInteractive
#include <ftxui/dom/elements.hpp>                 // ftxui::hbox, ftxui::text, ftxui::border, ftxui::flex
#include <ftxui/dom/table.hpp>                    // ftxui::Table
#include <iomanip>                                // std::setprecision
#include <optional>                               // std::optional

#include "../common/constants.hpp"                                 // k_mem_units
#include "../common/conversions.h"                                 // GIGABYTES, KILOBYTES
#include "../common/formatting/system_info_provider.interface.hpp" // ISystemInfoProvider
#include "ftxui_helpers.hpp"                                       // FtxUiHelpers
#include "sys_info/printer.interface.hpp"                          // IPrinter

using namespace ftxui;
using namespace sys_format;

/**
 * @brief FTXUI-based terminal printer for system monitoring data.
 *
 * Owns an interactive fullscreen terminal screen and a system information
 * provider used to format collected data for rendering. The printer starts the
 * FTXUI event loop on the first print call and posts redraw events on
 * subsequent calls.
 *
 * The printer is not copyable or movable because it owns a screen, a UI loop
 * thread, and a provider with a lifetime tied to this printer instance.
 */
class FtxUiPrinter final : public IPrinter
{
private:
    /** @brief Maximum CPU columns display count */
    constexpr static size_t k_max_cpu_columns = 4;

    /** @brief Process table column names and optional decorators. */
    static const std::array<std::tuple<std::string, std::optional<ftxui::Decorator>>, 6> m_header_names;

public:
    /**
     * @brief Creates an FTXUI printer with the provided system information provider.
     *
     * @param provider_ptr System information provider owned by the printer.
     */
    explicit FtxUiPrinter(std::unique_ptr<ISystemInfoProvider>);

    FtxUiPrinter(const FtxUiPrinter&) = delete;
    FtxUiPrinter(FtxUiPrinter&&) = delete;

    /** @brief Stops the UI loop thread and releases printer resources. */
    ~FtxUiPrinter() override;

    /**
     * @copydoc IPrinter::print(SystemInfo&)
     *
     * @brief Starts or refreshes the terminal rendering loop.
     */
    void print(SystemInfo&) override;

    FtxUiPrinter& operator=(const FtxUiPrinter&) = delete;
    FtxUiPrinter& operator=(FtxUiPrinter&&) = delete;

private:
    /**
     * @brief Builds the complete terminal layout.
     *
     * @param system_info System information snapshot used by the system summary panel.
     *
     * @return Root FTXUI element for the current frame.
     */
    Element build_layout(SystemInfo&);

    /**
     * @brief Builds the system summary panel.
     *
     * @param system_info System information snapshot rendered in the panel.
     *
     * @return FTXUI element containing formatted system information.
     */
    Element build_system(const SystemInfo&);

    /**
     * @brief Builds the memory usage panel.
     *
     * @return FTXUI element containing formatted memory statistics.
     */
    Element build_mem();

    /**
     * @brief Builds the CPU usage panel.
     *
     * @return FTXUI element containing formatted CPU statistics.
     */
    Element build_cpu();

    /**
     * @brief Builds the processes table.
     *
     * @return FTXUI element containing formatted process information.
     */
    Element build_process();

    /** @brief Stops the FTXUI loop thread when it is running. */
    void stop_thread();

private:
    /** @brief Interactive fullscreen terminal screen managed by FTXUI. */
    ScreenInteractive m_screen;

    /** @brief Indicates whether the FTXUI loop thread has been started. */
    std::atomic_bool m_initialized;

    /** @brief Thread that owns the blocking FTXUI event loop. */
    std::thread m_loop_thread;

    /** @brief Cached number of CPU columns used to choose the layout shape. */
    std::optional<size_t> m_cpu_columns_count;

    /** @brief System information provider owned by this printer. */
    std::unique_ptr<ISystemInfoProvider> m_provider_ptr;
};

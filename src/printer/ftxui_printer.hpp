#pragma once

#include <atomic> // std::atomic_bool
#include <cmath> // std::ceil
#include <csignal> // std::raise, SIGABRT
#include "ftxui/component/component.hpp" // ftxui::Container
#include <ftxui/component/screen_interactive.hpp> // ftxui::ScreenInteractive
#include <ftxui/dom/elements.hpp> // ftxui::hbox, ftxui::text, ftxui::border, ftxui::flex
#include <iomanip> // std::setprecision
#include <optional> // std::optional

#include "sys_info/printer.interface.hpp" // IPrinter
#include "ftxui_helpers.hpp" // FtxUiHelpers
#include "../common/constants.hpp" // k_mem_units
#include "../common/conversions.h" // GIGABYTES, KILOBYTES
#include "../utils/utils.h" // elapsed_time

using namespace ftxui;

// TODO: add descr
// TODO: copy + move
class FtxUiPrinter final : public IPrinter
{
private:
    /** @brief Maximum CPU columns display count */
    constexpr static size_t k_max_cpu_columns = 4;
//    constexpr static size_t k_max_cpu_columns = 4;
//    constexpr static size_t k_max_cpu_columns = 4; TODO: check
//    constexpr static size_t k_max_cpu_columns = 4;

public:
    FtxUiPrinter() : m_screen{ScreenInteractive::Fullscreen()}, m_initialized{false}
    {}

    ~FtxUiPrinter() override
    {
        if (m_initialized.load())
        {
            m_loop_thread.join();
        }
    }

    void print(SystemInfo&) override;

private:
    Element build_layout(SystemInfo&);

    Element build_system(const SystemInfo&);

    Element build_mem(const SystemInfo&);

    Element build_cpu(const SystemInfo&);

    Element build_process(const SystemInfo&);

    ScreenInteractive m_screen;

    std::atomic_bool m_initialized;

    std::thread m_loop_thread;

    std::optional<size_t> m_cpu_columns_count;
};


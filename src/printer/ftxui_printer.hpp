#pragma once

#include "ftxui/component/component.hpp"          // ftxui::Container
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
#include "../utils/utils.h"                                        // elapsed_time
#include "ftxui_helpers.hpp"                                       // FtxUiHelpers
#include "sys_info/printer.interface.hpp"                          // IPrinter

using namespace ftxui;
using namespace sys_format;

// TODO: add descr
// TODO: copy + move
class FtxUiPrinter final : public IPrinter
{
private:
    /** @brief Maximum CPU columns display count */
    constexpr static size_t k_max_cpu_columns = 4;

    static const std::array<std::string, 6> m_header_names;

public:
    FtxUiPrinter(const std::shared_ptr<ISystemInfoProvider>& provider_ptr)
        : m_screen{ScreenInteractive::Fullscreen()},
          m_initialized{false},
          m_provider_ptr{provider_ptr}
    {
    }

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

    Element build_mem(const std::shared_ptr<ISystemInfoProvider>&);

    Element build_cpu(const std::shared_ptr<ISystemInfoProvider>&);

    Element build_process(const SystemInfo&);

    ScreenInteractive m_screen;

    std::atomic_bool m_initialized;

    std::thread m_loop_thread;

    std::optional<size_t> m_cpu_columns_count;

    std::weak_ptr<ISystemInfoProvider> m_provider_ptr;
};

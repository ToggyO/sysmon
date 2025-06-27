#pragma once

#include <atomic> // std::atomic_bool
#include <csignal> // std::raise, SIGABRT
#include "ftxui/component/component.hpp" // ftxui::Container
#include <ftxui/component/screen_interactive.hpp> // ftxui::ScreenInteractive
#include <ftxui/dom/elements.hpp> // ftxui::hbox, ftxui::text, ftxui::border, ftxui::flex

#include "sys_info/printer.interface.hpp" // IPrinter
#include "../../common/constants.hpp" // k_mem_units
#include "../../common/conversions.h" // GIGABYTES, KILOBYTES
#include "../../common/ui_helpers.h" // UiHelpers::build_progress_bar
#include "../../utils/utils.h" // elapsed_time

using namespace ftxui;

// TODO: add descr
class FtxUiPrinter final : public IPrinter
{
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

    void print(const SystemInfo&) override;

private:
    static Element build_layout(const SystemInfo&);

    ScreenInteractive m_screen;
    std::atomic_bool m_initialized;
    std::thread m_loop_thread;
};
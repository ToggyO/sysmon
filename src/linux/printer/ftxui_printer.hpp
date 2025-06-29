#pragma once

#include <atomic> // std::atomic_bool
#include <csignal> // std::raise, SIGABRT
#include "ftxui/component/component.hpp" // ftxui::Container
#include <ftxui/component/screen_interactive.hpp> // ftxui::ScreenInteractive
#include <ftxui/dom/elements.hpp> // ftxui::hbox, ftxui::text, ftxui::border, ftxui::flex
#include <iomanip> // std::setprecision

#include "sys_info/printer.interface.hpp" // IPrinter
#include "../../common/constants.hpp" // k_mem_units
#include "../../common/conversions.h" // GIGABYTES, KILOBYTES
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

    static Element build_system(const SystemInfo& system_info);

    static Element build_cpu(const SystemInfo& system_info);

    static Element build_process(const SystemInfo& system_info);

    static Decorator get_gauge_color(const double& percent);

    ScreenInteractive m_screen;
    std::atomic_bool m_initialized;
    std::thread m_loop_thread;

    // TODO: объявить и вынести в отдельны файл
    struct PrintingConstants
    {
        static constexpr char k_percent = '%';

        inline static const std::string k_os = "OS: ";
        inline static const std::string k_uptime = "Up Time: ";
        inline static const std::string k_mem = "Mem: ";
    };

    // TODO: объявить и вынести в отдельны файл
    struct CommonColors
    {
        struct Text
        {
            static constexpr Color::Palette256 title_text = Color::Palette256::Green3Bis;
            static constexpr Color::Palette256 value_text = Color::Palette256::Khaki1;
        };

        struct ImportanceLevel
        {
            static constexpr Color::Palette256 normal = Color::Palette256::DarkSeaGreen4Bis;
            static constexpr Color::Palette256 medium = Color::Palette256::Yellow1;
            static constexpr Color::Palette256 critical = Color::Palette256::IndianRed;
        };
    };
};


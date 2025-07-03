#pragma once

#include <ftxui/dom/elements.hpp>

using namespace ftxui;

// TODO: add descr
struct FtxUiHelpers
{
    // TODO: add descr
    static Decorator get_gauge_color(const double& percent);

    static Element hindent(int);

    static Element vindent(int);

    struct PrintingConstants
    {
        static constexpr char k_percent = '%';

        inline static const std::string k_os = "OS: ";
        inline static const std::string k_uptime = "Up Time: ";
        inline static const std::string k_mem = "Mem: ";
    };

    struct CommonColors
    {
        struct Text
        {
//            static constexpr Color::Palette256 title_text = Color::Palette256::Green3Bis; TODO: check
            static constexpr Color::Palette16 title_text = Color::White;
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
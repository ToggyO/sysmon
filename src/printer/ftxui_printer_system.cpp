#include "ftxui_printer.hpp"

Element FtxUiPrinter::build_system(const SystemInfo &system_info)
{
    std::string uptime;
    elapsed_time(uptime, system_info.uptime);

    return vbox({
        hbox({
            text(FtxUiHelpers::PrintingConstants::k_os) | color(FtxUiHelpers::CommonColors::Text::title_text),
            text(system_info.get_os_name()) |
            color(FtxUiHelpers::CommonColors::Text::value_text),
        }),
        hbox({
            text(FtxUiHelpers::PrintingConstants::k_uptime) |
            color(FtxUiHelpers::CommonColors::Text::title_text),
            text(uptime) | color(FtxUiHelpers::CommonColors::Text::value_text),
        })
    }) | flex | size(WidthOrHeight::WIDTH, EQUAL, Terminal::Size().dimx / 2);
}
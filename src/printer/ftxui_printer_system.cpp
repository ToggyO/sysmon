#include "../utils/utils.h" // elapsed_time
#include "ftxui_printer.hpp"

Element FtxUiPrinter::build_system(const SystemInfo& system_info)
{
    std::string uptime;
    elapsed_time(uptime, system_info.uptime);

    std::stringstream load_avg_values;
    load_avg_values << std::fixed << std::setprecision(2) << system_info.load_average_stats.load_average_1 << ' '
                    << system_info.load_average_stats.load_average_5 << ' '
                    << system_info.load_average_stats.load_average_15;

    return vbox({hbox({
                     text(FtxUiHelpers::PrintingConstants::k_os) | color(FtxUiHelpers::CommonColors::Text::title_text),
                     text(system_info.get_os_name()) | color(FtxUiHelpers::CommonColors::Text::value_text),
                 }),
                 hbox({
                     text(FtxUiHelpers::PrintingConstants::k_uptime) |
                         color(FtxUiHelpers::CommonColors::Text::title_text),
                     text(uptime) | color(FtxUiHelpers::CommonColors::Text::value_text),
                 }),
                 hbox({text(FtxUiHelpers::PrintingConstants::k_load_avg) |
                           color(FtxUiHelpers::CommonColors::Text::title_text),
                       text(load_avg_values.str()) | color(FtxUiHelpers::CommonColors::Text::value_text)})}) |
           flex | size(WidthOrHeight::WIDTH, EQUAL, Terminal::Size().dimx / 2) | border;
}

#include "ftxui_printer.hpp"

Element FtxUiPrinter::build_mem(const SystemInfo& system_info)
{
    const double total_memory = convert_unit(system_info.memory_stats.total_memory, GIGABYTES, KILOBYTES);
    const double used_memory = convert_unit(system_info.memory_stats.used_memory, GIGABYTES, KILOBYTES);
    const double percentage = used_memory / total_memory;

    std::stringstream mem_text;
    mem_text << " " << std::fixed << std::setprecision(2) << used_memory << '/'
         << std::fixed << std::setprecision(2) << total_memory << Constants::k_mem_units;

    auto gauge_color = FtxUiHelpers::get_gauge_color(percentage * 100);

    // TODO: add SWAP bar
    auto r = mem_text.str();
    return vbox({
        hbox({
            vbox({
                filler(),
                text(FtxUiHelpers::PrintingConstants::k_mem)
                    | align_right
                    | color(FtxUiHelpers::CommonColors::Text::title_text)
                    | size(WidthOrHeight::WIDTH, EQUAL, 4),
                filler(),
            }),
            hbox({
                gauge((float) percentage)
                    | gauge_color
                    | size(WidthOrHeight::WIDTH, EQUAL, Terminal::Size().dimx / 2 - 20), // TODO: тут поиграться
                text(mem_text.str()) | gauge_color,
            }),
        })
    });
}

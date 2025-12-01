#include "ftxui_printer.hpp"

Element build_bar(const std::string& title, const float percentage, const Decorator & gauge_color, std::string&& value_text)
{
    return hbox({
        vbox({
            filler(),
            text(title)
                | align_right
                | color(FtxUiHelpers::CommonColors::Text::title_text)
                | size(WidthOrHeight::WIDTH, EQUAL, 4),
            filler(),
        }),
        hbox({
            gauge((float) percentage)
                | gauge_color
                | size(WidthOrHeight::WIDTH, EQUAL, Terminal::Size().dimx / 2 - 21),
            text(std::move(value_text))
                | align_right
                | gauge_color
                | size(WidthOrHeight::WIDTH, EQUAL, 14),
        }),
    });
}

void build_mem_value_text(std::stringstream& ss, const double& used, const double& total)
{
    ss << ' ' << std::fixed << std::setprecision(2) << used << Constants::k_mem_units << '/'
        << std::fixed << std::setprecision(2) << total << Constants::k_mem_units;
}

Element FtxUiPrinter::build_mem(const std::shared_ptr<ISystemInfoProvider>& provider_ptr)
{
    const auto& mem_formatter = provider_ptr->get_mem_provider();

    // RAM bar
    const double total_memory = mem_formatter.get_converted_mem_value(RAMTotalOrUsed::TOTAL, BYTE_UNITS::GIGABYTES);
    const double used_memory = mem_formatter.get_converted_mem_value(RAMTotalOrUsed::USED, BYTE_UNITS::GIGABYTES);
    const double mem_usage_percentage = used_memory / total_memory;

    std::stringstream mem_text;
    build_mem_value_text(mem_text, used_memory, total_memory);

    auto mem_gauge_color = FtxUiHelpers::get_gauge_color(mem_usage_percentage * 100);

    // Swap bar
    const double total_swap = mem_formatter.get_converted_swap_value(SwapTotalOrFree::TOTAL, BYTE_UNITS::GIGABYTES);
    const double free_swap = mem_formatter.get_converted_swap_value(SwapTotalOrFree::FREE, BYTE_UNITS::GIGABYTES);
    const double used_swap = total_swap - free_swap;
    double swap_usage_percentage = (double)used_swap / total_swap;

    if (std::isnan(swap_usage_percentage))
    {
        swap_usage_percentage = 0;
    }
    auto swap_gauge_color = FtxUiHelpers::get_gauge_color(swap_usage_percentage * 100);

    std::stringstream swap_text;
    build_mem_value_text(swap_text, used_swap, total_swap);

    return vbox({
        build_bar(FtxUiHelpers::PrintingConstants::k_mem, mem_usage_percentage, mem_gauge_color, mem_text.str()),
        FtxUiHelpers::vindent(1),
        build_bar(FtxUiHelpers::PrintingConstants::k_swap, swap_usage_percentage, swap_gauge_color, swap_text.str())
    }) | size(WidthOrHeight::WIDTH, EQUAL, Terminal::Size().dimx / 2) | border;
}

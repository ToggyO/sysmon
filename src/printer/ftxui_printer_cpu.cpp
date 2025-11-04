#include "ftxui_printer.hpp"

namespace
{
    void push_back(Elements& elements, Elements&& cpu_box)
    {
        if (elements.size() != 0)
        {
            elements.push_back(FtxUiHelpers::hindent(1));
        }

        elements.push_back(vbox(std::move(cpu_box)) | flex);

        if (elements.size() != elements.capacity())
        {
            elements.push_back(FtxUiHelpers::hindent(1));
        }
    }
}

Element FtxUiPrinter::build_cpu(const SystemInfo& system_info)
{
    // < 4 - 1 column
    // < 7 - 2 columns
    // >= 7 - 4 columns
    auto cpu_count = system_info.cpu_load_collection.size();
    if (cpu_count < 4)
    {
        m_cpu_columns_count = 1;
    }
    else if (cpu_count < 7)
    {
        m_cpu_columns_count = 2;
    }
    else
    {
        m_cpu_columns_count = k_max_cpu_columns;
    }

    auto max_cpu_bars_per_column = std::ceil((float)cpu_count / m_cpu_columns_count.value());

    // Reserving space for indentation elements: columns count + 2 indents * colmns count - (ultra-left indent + ultra-right indent)
    Elements elements;
    elements.reserve(m_cpu_columns_count.value() + 2 * m_cpu_columns_count.value() - 2);

    Elements cpu_box(max_cpu_bars_per_column, emptyElement());

    std::stringstream ss;
    uint8_t current_cpu_box_elements_count = 0;
    uint8_t i = 0;

    for (const auto& cpu_load : system_info.cpu_load_collection)
    {
        auto percentage = cpu_load.usage_percentage * 100;
        auto gauge_color = FtxUiHelpers::get_gauge_color(percentage);
        ss << std::fixed << std::setprecision(2) << percentage << FtxUiHelpers::PrintingConstants::k_percent;

        if (current_cpu_box_elements_count == max_cpu_bars_per_column) // TODO: check condition. mb < or >
        {
            push_back(elements, std::move(cpu_box));
            cpu_box = Elements(max_cpu_bars_per_column, emptyElement());
            current_cpu_box_elements_count = 0;
        }

        Elements item;
        if (current_cpu_box_elements_count != 0)
        {
            item.push_back(FtxUiHelpers::vindent(1));
        }

        item.push_back(hbox({
            vbox({
                text(std::to_string(i) + ':')
                    | align_right
                    | color(FtxUiHelpers::CommonColors::Text::title_text)
                    | size(WidthOrHeight::WIDTH, EQUAL, 4)
            }),
            hbox({
                gauge((float)cpu_load.usage_percentage) | gauge_color,
                    text(ss.str())
                        | align_right
                        | gauge_color
                        | size(WidthOrHeight::WIDTH, EQUAL, 8),
                }) | flex
            }));

        cpu_box[current_cpu_box_elements_count] = vbox(std::move(item));

        i++;
        ss.clear();
        ss.str("");
        current_cpu_box_elements_count++;
    }

    push_back(elements, std::move(cpu_box));
    return hbox(std::move(elements)) | flex | border;
}
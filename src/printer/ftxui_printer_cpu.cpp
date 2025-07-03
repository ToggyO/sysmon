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

// TODO: описать, как это дерьмо с наполнением работает
Element FtxUiPrinter::build_cpu(const SystemInfo& system_info)
{
    // < 4 - 1 column
    // < 7 - 2 columns
    // >= 7 - 4 columns
    // TODO: достаточно один раз посчитать, не?
    auto cpu_count = system_info.cpu_load_collection.size();
    if (cpu_count < 4) // TODO: в константы
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
//    elements.reserve(m_cpu_columns_count.value() + 2 * m_cpu_columns_count.value()); TODO: check
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
                text(std::to_string(i) + ':') // TODO: оптимизировать
                    | align_right
                    | color(FtxUiHelpers::CommonColors::Text::title_text)
                    | size(WidthOrHeight::WIDTH, EQUAL, 4) // TODO: проверить константу
            }),
            hbox({
                gauge((float)cpu_load.usage_percentage) | gauge_color,
                    text(ss.str())
                        | align_right
                        | gauge_color
                        | size(WidthOrHeight::WIDTH, EQUAL, 8), // TODO: тут текст процентажа смещает gauge влево при увелчении порядка
                }) | flex
            }));

        cpu_box[current_cpu_box_elements_count] = vbox(std::move(item));

        i++;
        ss.clear();
        ss.str("");
        current_cpu_box_elements_count++;
    }

    push_back(elements, std::move(cpu_box));
    return hbox(std::move(elements)) | flex;
}



//Element FtxUiPrinter::build_cpu(const SystemInfo& system_info)
//{
//    Elements elements;
//    std::stringstream ss;
//
//    size_t max_cpu_column_size = 4;
//    Elements cpu_box(max_cpu_column_size, Element{});
//    uint8_t i = 0;
//    uint8_t current_cpu_box_elements_count = 0;
//    for (const auto & cpu_load : system_info.cpu_load_collection)
//    {
//        auto percentage = cpu_load.usage_percentage * 100;
//        auto gauge_color = FtxUiHelpers::get_gauge_color(percentage);
//        ss << std::fixed << std::setprecision(2) << percentage << FtxUiHelpers::PrintingConstants::k_percent;
//
//        if (current_cpu_box_elements_count == max_cpu_column_size) // TODO: check condition. mb < or >
//        {
////            elements.push_back(vbox(std::move(cpu_box)) | bgcolor(Color::Palette256::BlueViolet));
////            elements.push_back(flexbox(std::move(cpu_box), FlexboxConfig()
////                    .Set(FlexboxConfig::Direction::Column)
////                    .Set(FlexboxConfig::Wrap::NoWrap)
////                    .SetGap(0, 1))); // TODO: duplicate
//            elements.push_back(vbox(std::move(cpu_box))); // TODO: duplicate
//            elements.push_back(vbox(std::move(cpu_box))); // TODO: duplicate
//
//            cpu_box = Elements(4, Element{});
//            current_cpu_box_elements_count = 0;
//        }
//
//        // TODO: дупликация формирования gauge
//        cpu_box[current_cpu_box_elements_count] = flexbox({
//            vbox({
//                text(std::to_string(i) + ':') // TODO: оптимизировать
//                    | align_right
//                    | color(FtxUiHelpers::CommonColors::Text::title_text)
//                    | size(WidthOrHeight::WIDTH, EQUAL, 3) // TODO: проверить константу
//            }),
//            hbox({
//                gauge((float)cpu_load.usage_percentage) | gauge_color,
//                text(ss.str()) | gauge_color, // TODO: тут текст процентажа смещает gauge влево при увелчении порядка
//            }) | size(WidthOrHeight::WIDTH, GREATER_THAN, Terminal::Size().dimx / (system_info.cpu_load_collection.size() / max_cpu_column_size) - 10) // TODO: check
////                | size(WidthOrHeight::HEIGHT, EQUAL, 2)
//                                                          });
////                | size(WidthOrHeight::WIDTH, EQUAL, 25) | size(WidthOrHeight::HEIGHT, EQUAL, 2); // TODO: проверить константу
////        cpu_box[current_cpu_box_elements_count] = hbox({
////            vbox({
////                filler(),
////                text(std::to_string(i))
////                    | color(CommonColors::Text::title_text)
////                    | size(WidthOrHeight::WIDTH, EQUAL, 3) // TODO: проверить константу
////                    ,
////                filler(),
////            }),
////            hbox({
////                gauge((float)cpu_load.usage_percentage) | gauge_color,
////                text(ss.str()) | gauge_color,
////            }) | flex
////        }) | size(WidthOrHeight::WIDTH, EQUAL, 25) | size(WidthOrHeight::HEIGHT, EQUAL, 2); // TODO: проверить константу
////        }) | size(WidthOrHeight::HEIGHT, EQUAL, 2); // TODO: проверить константу
//
////        if (i != system_info.cpu_load_collection.size() - 1)
////        {
//////            elements.push_back(filler()); // TODO: remove
//////            elements.push_back(separator());
////        }
////        if (current_cpu_box_elements_count < max_cpu_column_size)
////        {
////
////        }
//
//        i++;
//        ss.clear();
//        ss.str("");
//        current_cpu_box_elements_count++;
//    }
//
////    elements.push_back(vbox(std::move(cpu_box)));
//    elements.push_back(flexbox(std::move(cpu_box), FlexboxConfig()
//            .Set(FlexboxConfig::Direction::Column)
//            .Set(FlexboxConfig::Wrap::NoWrap)
//            .SetGap(0, 1))); // TODO: duplicate
//    return hbox({
//        FtxUiHelpers::margin(2),
//        vbox({}) | border | flex,
//        FtxUiHelpers::margin(2),
//        vbox({}) | border | flex,
//        FtxUiHelpers::margin(2),
//        vbox({}) | border | flex,
//        FtxUiHelpers::margin(2),
//        vbox({}) | border | flex,
//        FtxUiHelpers::margin(2),
//    });
//
//
////    elements.push_back(flexbox(std::move(cpu_box), FlexboxConfig()
////            .Set(FlexboxConfig::Direction::Column)
////            .SetGap(0, 1))); // TODO: duplicate
////    return hbox(elements); // TODO: remove
////    return flexbox(std::move(elements), FlexboxConfig()
////            .Set(FlexboxConfig::JustifyContent::SpaceBetween)
////            .Set(FlexboxConfig::AlignContent::SpaceBetween));
////    return hbox({
////                        FtxUiHelpers::margin(2),
////                        vbox({}) | border | flex,
////                        FtxUiHelpers::margin(2),
////                        vbox({}) | border | flex,
////                        FtxUiHelpers::margin(2),
////                        vbox({}) | border | flex,
////                        FtxUiHelpers::margin(2),
////                        vbox({}) | border | flex,
////                        FtxUiHelpers::margin(2),
////                });
//}

// TODO: check
//Element FtxUiPrinter::build_cpu(const SystemInfo& system_info)
//{
//    Elements elements;
//    std::stringstream ss;
//
//    uint8_t i = 0;
//    for (const auto & cpu_load : system_info.cpu_load_collection)
//    {
//        auto gauge_color = get_gauge_color(cpu_load.usage_percentage);
//        ss << std::fixed << std::setprecision(2) << cpu_load.usage_percentage * 100 << PrintingConstants::k_percent;
//
//        // TODO: дупликация формирования gauge
//        elements.push_back(
//            hbox({
//                vbox({
//                    filler(),
//                    text(std::to_string(i))
//                        | color(CommonColors::Text::title_text)
//                        | size(WidthOrHeight::WIDTH, EQUAL, 3) // TODO: проверить константу
//                        | align_right,
//                    filler(),
//                }),
//                hbox({
//                    gauge((float)cpu_load.usage_percentage) | gauge_color,
//                    text(ss.str()) | gauge_color,
//                }) | flex
//            }) | size(WidthOrHeight::WIDTH, EQUAL, 30)
//        );
//
//        if (i != system_info.cpu_load_collection.size() - 1)
//        {
////            elements.push_back(filler()); // TODO: remove
////            elements.push_back(separator());
//        }
//
//        i++;
//        ss.clear();
//        ss.str("");
//    }
//
////    return flexbox(std::move(elements), FlexboxConfig().Set(FlexboxConfig::Wrap::Wrap).Set(FlexboxConfig::AlignContent::Stretch));
//    return flexbox(std::move(elements), FlexboxConfig()
//        .Set(FlexboxConfig::Wrap::Wrap)
//        .Set(FlexboxConfig::JustifyContent::SpaceBetween));
////    return flexbox(std::move(elements)); // TODO: remvove
////    return vbox(std::move(elements));
//}
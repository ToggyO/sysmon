#include "ftxui_printer.hpp"

// TODO: разбить на файлы для удобочитаемости?
void FtxUiPrinter::print(const SystemInfo& system_info)
{
    if (!m_initialized.load())
    {
        m_loop_thread = std::thread([&]
        {
            const auto renderer = Renderer([&] { return build_layout(system_info); });
            this->m_initialized.store(true);
            this->m_screen.Loop(renderer); // Blocking call
            std::raise(SIGABRT); // TODO: костыль, потому что обработчик сигналов ftxui скрывает поступления сигнала от OS для основного приложения
        });
        return;
    }

    // Causing ftxui screen redraw
    m_screen.PostEvent(Event::Custom);
}

Element FtxUiPrinter::build_layout(const SystemInfo& system_info)
{
    return vbox({
        hbox({
//            build_system(system_info) | border | flex | size(WidthOrHeight::WIDTH, EQUAL, Terminal::Size().dimx / 3),
            build_cpu(system_info) | border | flex,
        }) | size(WidthOrHeight::HEIGHT, LESS_THAN, 10),
        build_process(system_info) | border | flex
    }) | flex;
}

Element FtxUiPrinter::build_system(const SystemInfo& system_info)
{
    std::string uptime;
    elapsed_time(uptime, system_info.uptime);

    const double total_memory = convert_unit(system_info.memory_stats.total_memory, GIGABYTES, KILOBYTES);
    const double used_memory = convert_unit(system_info.memory_stats.used_memory, GIGABYTES, KILOBYTES);
    const double percentage = used_memory / total_memory;

    std::stringstream mem_text;
    mem_text << " " << std::fixed << std::setprecision(2) << used_memory<< '/'
        << std::fixed << std::setprecision(2) << total_memory << Constants::k_mem_units;

    auto gauge_color = get_gauge_color(percentage * 100);
    return vbox({
        hbox({
            text(PrintingConstants::k_os) | color(CommonColors::Text::title_text),
            text(system_info.get_os_name()) | color(CommonColors::Text::value_text),
        }),
        hbox({
            text(PrintingConstants::k_uptime) | color(CommonColors::Text::title_text),
            text(uptime) | color(CommonColors::Text::value_text),
        }),
        hbox({
            vbox({
                filler(),
                text(PrintingConstants::k_mem) | color(CommonColors::Text::title_text),
                filler(),
            }),
            hbox({ // TODO: дупликация формирования gauge
                gauge((float)percentage) | gauge_color,
                text(mem_text.str()) | gauge_color | size(WidthOrHeight::WIDTH, EQUAL, 15),
            }) | flex | border | color(CommonColors::Text::value_text),
        }),
    });
}

// TODO: описать, как это дерьмо с наполнением работает
Element FtxUiPrinter::build_cpu(const SystemInfo& system_info)
{
    Elements elements;
    std::stringstream ss;

    size_t max_cpu_column_size = 4;
    Elements cpu_box(max_cpu_column_size, Element{});
    uint8_t i = 0;
    uint8_t current_cpu_box_elements_count = 0;
    for (const auto & cpu_load : system_info.cpu_load_collection)
    {
        auto percentage = cpu_load.usage_percentage * 100;
        auto gauge_color = get_gauge_color(percentage);
        ss << std::fixed << std::setprecision(2) << percentage << PrintingConstants::k_percent;

        if (current_cpu_box_elements_count == max_cpu_column_size) // TODO: check condition. mb < or >
        {
//            elements.push_back(vbox(std::move(cpu_box)) | bgcolor(Color::Palette256::BlueViolet));
            elements.push_back(flexbox(std::move(cpu_box), FlexboxConfig()
                .Set(FlexboxConfig::Direction::Column)
                .Set(FlexboxConfig::Wrap::NoWrap)
                .SetGap(0, 1))); // TODO: duplicate
            cpu_box = Elements(4, Element{});
            current_cpu_box_elements_count = 0;
        }

        // TODO: дупликация формирования gauge
        cpu_box[current_cpu_box_elements_count] = flexbox({
            vbox({
                text(std::to_string(i) + ':') // TODO: оптимизировать
                    | align_right
                    | color(CommonColors::Text::title_text)
                    | size(WidthOrHeight::WIDTH, EQUAL, 3)// TODO: проверить константу
                ,
            }),
            hbox({
//                text("["), // TODO: оптимизировать
                gauge((float)cpu_load.usage_percentage) | gauge_color,
                text(ss.str()) | gauge_color, // TODO: тут текст процентажа смещает gauge влево при увелчении порядка
//                text("]") // TODO: оптимизировать
            }) | size(WidthOrHeight::WIDTH, GREATER_THAN, Terminal::Size().dimx / (system_info.cpu_load_collection.size() / max_cpu_column_size) - 10) // TODO: check
//                | size(WidthOrHeight::HEIGHT, EQUAL, 2)
       });
//                | size(WidthOrHeight::WIDTH, EQUAL, 25) | size(WidthOrHeight::HEIGHT, EQUAL, 2); // TODO: проверить константу
//        cpu_box[current_cpu_box_elements_count] = hbox({
//            vbox({
//                filler(),
//                text(std::to_string(i))
//                    | color(CommonColors::Text::title_text)
//                    | size(WidthOrHeight::WIDTH, EQUAL, 3) // TODO: проверить константу
//                    ,
//                filler(),
//            }),
//            hbox({
//                gauge((float)cpu_load.usage_percentage) | gauge_color,
//                text(ss.str()) | gauge_color,
//            }) | flex
//        }) | size(WidthOrHeight::WIDTH, EQUAL, 25) | size(WidthOrHeight::HEIGHT, EQUAL, 2); // TODO: проверить константу
//        }) | size(WidthOrHeight::HEIGHT, EQUAL, 2); // TODO: проверить константу

//        if (i != system_info.cpu_load_collection.size() - 1)
//        {
////            elements.push_back(filler()); // TODO: remove
////            elements.push_back(separator());
//        }
//        if (current_cpu_box_elements_count < max_cpu_column_size)
//        {
//
//        }

        i++;
        ss.clear();
        ss.str("");
        current_cpu_box_elements_count++;
    }

//    elements.push_back(vbox(std::move(cpu_box)));
    elements.push_back(flexbox(std::move(cpu_box), FlexboxConfig()
            .Set(FlexboxConfig::Direction::Column)
            .Set(FlexboxConfig::Wrap::NoWrap)
            .SetGap(0, 1))); // TODO: duplicate
//    elements.push_back(flexbox(std::move(cpu_box), FlexboxConfig()
//            .Set(FlexboxConfig::Direction::Column)
//            .SetGap(0, 1))); // TODO: duplicate
//    return hbox(elements); // TODO: remove
    return flexbox(std::move(elements), FlexboxConfig()
            .Set(FlexboxConfig::JustifyContent::SpaceAround)
            .Set(FlexboxConfig::AlignContent::SpaceBetween));
}

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

Element FtxUiPrinter::build_process(const SystemInfo& system_info)
{
    return vbox({});
}

// Оно не работает
Decorator FtxUiPrinter::get_gauge_color(const double& percent)
{
    Color::Palette256 gauge_color;
    if (percent < 45) // TODO: пересмотреть и расширить границы
    {
        gauge_color = CommonColors::ImportanceLevel::normal;
    }
    else if (percent >= 45 && percent <= 85)
    {
        gauge_color = CommonColors::ImportanceLevel::medium;
    }
    else
    {
        gauge_color = CommonColors::ImportanceLevel::critical;
    }

    return color(gauge_color);
}
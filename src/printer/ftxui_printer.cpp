#include "ftxui_printer.hpp"

// TODO: разбить на файлы для удобочитаемости?
void FtxUiPrinter::print(SystemInfo& system_info)
{
    if (!m_initialized.load())
    {
        m_loop_thread = std::thread([&]
        {
            const auto renderer = Renderer([&] {
                return build_layout(system_info);
            });
            this->m_initialized.store(true);
            this->m_screen.Loop(renderer); // Blocking call
            std::raise(SIGABRT); // TODO: костыль, потому что обработчик сигналов ftxui скрывает поступления сигнала от OS для основного приложения
        });
        return;
    }

    // Causing ftxui screen redraw
    m_screen.PostEvent(Event::Custom);
}

Element FtxUiPrinter::build_layout(SystemInfo& system_info)
{
//    system_info.cpu_load_collection = std::vector<CpuLoad>(5, CpuLoad{}); // TODO: remove  - тест отображения cpu

    auto cpu_box = build_cpu(system_info);
    auto system_box = build_system(system_info);
    auto mem_box = build_mem(system_info);

    Element common_info_box;
    // TODO: может один раз посчитать, не?
    // TODO: проверка на optional?
    if (m_cpu_columns_count < 3)
    {
        common_info_box = hbox({
           vbox({
               std::move(cpu_box),
               FtxUiHelpers::vindent(1),
               std::move(mem_box),
           }),
           FtxUiHelpers::hindent(1),
           std::move(system_box)
        });
    }
    else
    {
        common_info_box = vbox({
            std::move(cpu_box),
            FtxUiHelpers::vindent(1),
            hbox({
                std::move(mem_box),
                FtxUiHelpers::hindent(4),
                std::move(system_box),
            })
        });
    }

    return hbox({
        FtxUiHelpers::hindent(2),
        vbox({
            std::move(common_info_box),
            build_process(system_info)
        }),
        FtxUiHelpers::hindent(2)
    });
}
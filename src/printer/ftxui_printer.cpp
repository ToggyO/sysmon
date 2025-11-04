#include "ftxui_printer.hpp"

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
            std::raise(SIGABRT); // Send abort singal to the application manually, because Ftxui signal hanler hides signals from OS
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

    if (m_cpu_columns_count < 3)
    {
        common_info_box = hbox({
           vbox({
               std::move(cpu_box),
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
            hbox({
                std::move(mem_box),
                FtxUiHelpers::hindent(1),
                std::move(system_box),
            })
        });
    }

    return hbox({
        FtxUiHelpers::hindent(2),
        vbox({
            FtxUiHelpers::vindent(1),
            std::move(common_info_box),
            build_process(system_info) | flex
        }),
        FtxUiHelpers::hindent(2)
    });
}

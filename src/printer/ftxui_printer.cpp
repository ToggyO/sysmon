#include "ftxui_printer.hpp"

void FtxUiPrinter::print(SystemInfo& system_info)
{
    if (!m_initialized.load())
    {
        m_loop_thread = std::thread(
            [&]
            {
                const auto renderer = Renderer([&] { return build_layout(system_info); });
                this->m_initialized.store(true);
                this->m_screen.Loop(renderer); // Blocking call
                std::raise(SIGABRT); // Send abort singal to the application manually, because Ftxui signal hanler hides
                                     // signals from OS
            });
        return;
    }

    // Causing ftxui screen redraw
    m_screen.PostEvent(Event::Custom);
}

// TODO: format boxes alignment
Element FtxUiPrinter::build_layout(SystemInfo& system_info)
{
    //    system_info.cpu_load_collection = std::vector<CpuLoad>(5, CpuLoad{}); // TODO: remove  - тест отображения cpu

    const auto sys_info_provider_ptr = m_provider_ptr.lock();
    if (!sys_info_provider_ptr)
    {
        throw std::runtime_error("FtxUiPrinter: system info formatter is required");
    }

    auto cpu_box = build_cpu(sys_info_provider_ptr);
    auto system_box = build_system(system_info);
    auto mem_box = build_mem(sys_info_provider_ptr);

    Element common_info_box;

    if (m_cpu_columns_count < 3)
    {
        common_info_box = hbox({vbox({
                                    std::move(cpu_box),
                                    std::move(mem_box),
                                }),
                                FtxUiHelpers::hindent(1), std::move(system_box)});
    }
    else
    {
        common_info_box = vbox({std::move(cpu_box), hbox({
                                                        std::move(mem_box),
                                                        FtxUiHelpers::hindent(1),
                                                        std::move(system_box),
                                                    })});
    }

    return hbox(
        {FtxUiHelpers::hindent(2),
         vbox({FtxUiHelpers::vindent(1), std::move(common_info_box), build_process(sys_info_provider_ptr) | flex}),
         FtxUiHelpers::hindent(2)});
}

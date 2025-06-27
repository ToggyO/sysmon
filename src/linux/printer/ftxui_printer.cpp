#include "ftxui_printer.hpp"

void FtxUiPrinter::print(const SystemInfo& system_info)
{
    if (!m_initialized.load())
    {
        // TODO: в отдельный метод?
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
    std::string mem_bar;
    const double total_memory = convert_unit(system_info.memory_stats.total_memory, GIGABYTES, KILOBYTES);
    const double used_memory = convert_unit(system_info.memory_stats.used_memory, GIGABYTES, KILOBYTES);
    UiHelpers::build_progress_bar(mem_bar, used_memory, total_memory, Constants::k_mem_units);

    std::string uptime;
    elapsed_time(uptime, system_info.uptime);

    return hbox({
        text(system_info.get_os_name())  | border,
        text(mem_bar) | border | flex,
        text(uptime) | border | flex,
    });
}
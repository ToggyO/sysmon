#include "ftxui_printer.hpp"

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

// TODO: метод класса
Element build_system(const SystemInfo& system_info)
{
    std::string uptime;
    elapsed_time(uptime, system_info.uptime);

    // std::string mem_bar;
    const double total_memory = convert_unit(system_info.memory_stats.total_memory, GIGABYTES, KILOBYTES);
    const double used_memory = convert_unit(system_info.memory_stats.used_memory, GIGABYTES, KILOBYTES);
    const double percent = used_memory / total_memory;

    std::stringstream mem_text;
    // TODO: бывает вот так - 12/31G вместо 12.0/31G
    // TODO: 11.9/31G надо превратить в 11.9/31.0G
    mem_text << " " << std::setprecision(3) << used_memory << '/' << std::setprecision(3) << total_memory << Constants::k_mem_units;
    // UiHelpers::build_progress_bar(mem_bar, used_memory, total_memory, Constants::k_mem_units);

    return vbox({
        hbox({
            text("OS: "), // TODO: в константы
            text(system_info.get_os_name()),
        }),
        hbox({
            text("Up Time: "), // TODO: в константы
            text(uptime),
        }),
        hbox({
            text("Mem: "), // TODO: в константы
            hbox({
                gauge(percent),
                text(mem_text.str()),
            }) | flex | border,
        }),
    });
}

// TODO: метод класса
Element build_cpu(const SystemInfo& system_info)
{
    return vbox({});
}

// TODO: метод класса
Element build_process(const SystemInfo& system_info) {
    return vbox({});
}

Element FtxUiPrinter::build_layout(const SystemInfo& system_info)
{
    return vbox({
        hbox({
            build_system(system_info) | border | flex,
            build_cpu(system_info) | border | flex,
        }),
        build_cpu(system_info) | border | flex
    });
}
#include "ftxui_printer.hpp"

// TODO: remove
//void FtxUiPrinter::print(const SystemInfo& system_info)
//{
////    std::cout << m_screen.ToString();
////    std::cout << m_screen.ResetPosition(/*clear=*/true);
////    std::cout << std::flush;
//
//    std::stringstream ss;
//    ss << system_info.memory_stats.used_memory << '/' << system_info.memory_stats.total_memory;
//
//    std::string uptime;
//    elapsed_time(uptime, system_info.uptime);
//
//    auto document = hbox({
//        text(system_info.get_os_name())  | border,
//        text(ss.str()) | border | flex,
//        text(uptime) | border,
//    });
//
////    auto document = hbox({});
//
//    Render(m_screen, document);
//
//    m_screen.Print();
//}

// TODO: метод класса?
void build_progress_bar(std::string &bar, double percent)
{
    // TODO: duplicate
    std::stringstream result;
    result <<  "0%";
    int size = 50;
    double bars = percent * size;

    for (int i = 0; i < size; ++i)
    {
        result << (i <= bars ? '|' : ' ');
    }

    std::string display = std::to_string(percent * 100).substr(0, 4);
    if (percent < 0.1 || percent == 1.0)
    {
        display = std::to_string(percent * 100).substr(0, 3);
    }

    result << " " << display << "/100%";
    bar = result.str();
}

void build_progress_bar(std::string &bar, double actual_value, double total_value, const std::string& units)
{
    // TODO: duplicate
    std::stringstream result;
    int size = 50;
    double percent = actual_value / total_value;
    double bars = percent * size;

    for (int i = 0; i < size; ++i)
    {
        result << (i <= bars ? '|' : ' ');
    }

    result << " " << std::setprecision(3) << actual_value << '/' << std::setprecision(3) << total_value << units;
    bar = result.str();
}

void FtxUiPrinter::print(const SystemInfo& system_info)
{
//    std::string mem_bar;
//    double total_memory = convert_unit(static_cast<double>(system_info.memory_stats.total_memory), GIGABYTES, KILOBYTES);
//    double used_memory = convert_unit(static_cast<double>(system_info.memory_stats.used_memory), GIGABYTES, KILOBYTES);
//    build_progress_bar(mem_bar, used_memory, total_memory, Constants::k_mem_units);

//    std::stringstream ss;
//    ss << system_info.memory_stats.used_memory << '/' << system_info.memory_stats.total_memory;
//
//    std::string uptime;
//    elapsed_time(uptime, system_info.uptime);

    if (!m_initialized.load())
    {
        // TODO: в отдельный метод?
        m_loop_thread = std::thread([&]
        {
            auto renderer = Renderer([&]()
            {
                std::string mem_bar;
                double total_memory = convert_unit(static_cast<double>(system_info.memory_stats.total_memory), GIGABYTES, KILOBYTES);
                double used_memory = convert_unit(static_cast<double>(system_info.memory_stats.used_memory), GIGABYTES, KILOBYTES);
                build_progress_bar(mem_bar, used_memory, total_memory, Constants::k_mem_units);

                std::string uptime;
                elapsed_time(uptime, system_info.uptime);

                return hbox({
                    text(system_info.get_os_name())  | border,
                    text(mem_bar) | border | flex,
                    text(uptime) | border | flex,
                });
            });

            this->m_initialized.store(true);
            this->m_screen.Loop(renderer);
//            std::cout << "Loop exits" << std::endl; // TODO: remove
            std::raise(SIGABRT); // TODO: костыль, потому что обработчик сигналов ftxui скрывает поступления сигнала от OS для основного приложения
        });
        return;
    }

    // Causing ftxui screen redraw
    m_screen.PostEvent(Event::Custom);

    // TODO: remove
//    if (!m_initialized.load())
//    {
//        std::cout << "Initializing FtxUIPrinter" << std::endl; // TODO: remove
//        auto renderer = Renderer([&]()
//         {
//             return hbox({
//                 text(system_info.get_os_name())  | border,
//                 text(ss.str()) | border | flex,
//                 text(uptime) | border | flex,
//             });
//         });
//
//        m_loop_ptr = std::make_unique<Loop>(&m_screen, renderer);
//        this->m_initialized.store(true);
//
//        // TODO: в отдельный метод?
//        m_loop_thread = std::thread([&]{});
//        return;
//    }


//    if (!m_loop_ptr->HasQuitted())
//    {
//        m_loop_ptr->RunOnce();
//    }

//    m_screen.RequestAnimationFrame();
//    m_screen.PostEvent(Event::Custom);
}
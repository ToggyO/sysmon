#include "ftxui_printer.hpp"

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

    std::stringstream ss;
    ss << system_info.memory_stats.used_memory << '/' << system_info.memory_stats.total_memory;

    std::string uptime;
    elapsed_time(uptime, system_info.uptime);

    if (!m_initialized.load())
    {
        // TODO: в отдельный метод?
        m_loop_thread = std::thread([&]
        {
            auto renderer = Renderer([&]()
            {
                return hbox({
                    text(system_info.get_os_name())  | border,
                    text(ss.str()) | border | flex,
                    text(uptime) | border | flex,
                });
            });

//            Event::CtrlC
            this->m_initialized.store(true);
            this->m_screen.Loop(renderer);
            std::cout << "Loop exits" << std::endl; // TODO: remove

//            auto k = m_screen.ExitLoopClosure();
//            m_screen.
                                        return;
            // TODO: Нужен грамотный выход из Loopa при нажатии ctrl+x< ctrl+d, ctrl+z - m_screen.ExitLoopClosure(), m_screen.Exit()
        });
        return;
    }

    m_screen.PostEvent(Event::Custom);
////    m_screen.Post([&] { return system_info; });
//
//
//    auto table = Table({});
//
//
//
//    auto component = Container::Vertical({renderer});
//
////    component->OnEvent()
//
//
//
//    component |= CatchEvent([&](const Event& event)
//    {
//        if (event == Event::Custom)
//        {
//
//            return true;
//        }
//        return false;
//    });
//
//
//    m_screen.Loop(component);
}
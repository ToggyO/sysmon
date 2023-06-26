#include "ncurses_printer.hpp"

static std::string k_mem_units = "G";
//const int pid_column = 2;
//const int user_column = 9;
//const int cpu_column = 16;
//const int ram_column = 26;
//const int time_column = 35;
//const int command_column = 46;
const int pid_column = 2;
const int user_column = 9;
const int cpu_column = 22;
const int ram_column = 30;
const int time_column = 42;
const int command_column = 54;

NCursesPrinter::NCursesPrinter(int n) // TODO: check n
{
    initscr(); // start ncurses
    use_default_colors();
//    noecho(); // do not print input values
//    cbreak(); // terminate ncurses on ctrl + c
    start_color(); // enable color
    int x_max = getmaxx(stdscr);
    int y_max = getmaxy(stdscr);
    int system_window_height = 9;

//    m_system_window = newwin(system_window_height, x_max - 1, 0, 0);
    m_system_window = newwin(system_window_height, (x_max / 2) - 1, 0, 0);
    m_cpu_window = newwin(system_window_height, x_max / 2, 0, x_max / 2);
    m_process_window = newwin(y_max - system_window_height, x_max - 1, m_system_window->_maxy, 0);
}

void NCursesPrinter::print(SystemInfo &system_info)
{
    init_pair(1, COLOR_BLUE, -1);
    init_pair(2, COLOR_GREEN, -1);
//    init_pair(3, COLOR_BLACK, COLOR_CYAN);

    box(m_system_window, 0, 0);
    box(m_cpu_window, 0, 0);
//    box(m_process_window, 0, 0);

    print_system(system_info);
    print_cpu(system_info);
    print_processes(system_info);

    wrefresh(m_system_window);
    wrefresh(m_cpu_window);
    wrefresh(m_process_window);
    refresh();
}

void NCursesPrinter::print_system(SystemInfo &system_info)
{
    int row = 0;
    mvwprintw(m_system_window, ++row, 2, "OS: %s", system_info.os_name.c_str());

    std::string uptime;
    elapsed_time(uptime, system_info.uptime);
    mvwprintw(m_system_window, ++row, 2, "Up Time: %s", uptime.c_str());

    mvwprintw(m_system_window, ++row, 2, "Mem: ");
    wattron(m_system_window, COLOR_PAIR(1));
    mvwprintw(m_system_window, row, 10, " ");
    std::string mem_bar;
    double total_memory = convert_unit(static_cast<double>(system_info.memory_stats.total_memory), GIGABYTES, KILOBYTES);
    double used_memory = convert_unit(static_cast<double>(system_info.memory_stats.used_memory), GIGABYTES, KILOBYTES);
    build_progress_bar(mem_bar, used_memory, total_memory, k_mem_units);
    wprintw(m_system_window, "%s", mem_bar.c_str());
    wattroff(m_system_window, COLOR_PAIR(1));

    wrefresh(m_system_window);
}

void NCursesPrinter::print_cpu(SystemInfo &system_info)
{
    //    for (int i = 4; i < 10; ++i)
//    {
//        system_info.cpu_load_collection.push_back(CpuLoad{std::to_string(i), 0.55});
//    }
    int row = 0;
//    int column;

    mvwprintw(m_cpu_window, ++row, 2, "CPU: ");
    wattron(m_cpu_window, COLOR_PAIR(1));

//    ++row;

    std::string cpu_bar;
    ushort i = 0;
    for (const auto & cpu_load : system_info.cpu_load_collection)
    {
//        if (i == 0 || i % 2 != 0)
//        {
//            column = 2; // TODO: в константы
//        }
//        else
//        {
//            column = 50; // TODO: в константы
//        }
//
//
//        build_progress_bar(cpu_bar, cpu_load.usage_percentage);
//        mvwprintw(m_cpu_window, row, column, "%d:  %s", i, cpu_bar.c_str());
//
//        if (column == 50)
//        {
//            ++row;
//        }
//        ++i;

        build_progress_bar(cpu_bar, cpu_load.usage_percentage);
        mvwprintw(m_cpu_window, ++row, 2, "%d:  %s", i, cpu_bar.c_str());
        ++i;


//        wprintw(m_cpu_window, "%s\n", cpu_bar.c_str());
//        wprintw(m_cpu_window, "%s: %s%%\n", cpu_load.cpu_id.c_str(), std::to_string(cpu_load.usage_percentage).c_str());
    }
    wattroff(m_cpu_window, COLOR_PAIR(1));

    wrefresh(m_cpu_window);
}

void NCursesPrinter::print_processes(SystemInfo &system_info)
{
    int row = 0;

//    wbkgd(m_process_window, COLOR_PAIR(3));
    wattron(m_process_window, COLOR_PAIR(2));
    mvwprintw(m_process_window, ++row, pid_column, "PID");
    mvwprintw(m_process_window, row, user_column, "USER");
    mvwprintw(m_process_window, row, cpu_column, "CPU[%%]");
    mvwprintw(m_process_window, row, ram_column, "RAM[MB]");
    mvwprintw(m_process_window, row, time_column, "TIME+");
    mvwprintw(m_process_window, row, command_column, "COMMAND");
    wattroff(m_process_window, COLOR_PAIR(2));

    mvwprintw(m_process_window, ++row, pid_column, "%s", (std::string(m_process_window->_maxx-2, ' ').c_str()));

    std::string process_uptime;
    std::stringstream formatter;
    fixed(formatter);
    formatter.precision(2);
    for (const auto& process : system_info.processes)
    {
        formatter.str("");
        //You need to take care of the fact that the cpu utilization has already been multiplied by 100.
        // Clear the line
        mvwprintw(m_process_window, ++row, pid_column, "%s", std::to_string(process.pid).c_str());
        mvwprintw(m_process_window, row, user_column, "%s", process.user.c_str());
        mvwprintw(m_process_window, row, cpu_column, "%s", std::to_string(process.cpu_usage * 100).substr(0, 4).c_str());

        formatter << convert_unit(static_cast<double>(process.memory_usage), MEGABYTES, KILOBYTES);
        mvwprintw(m_process_window, row, ram_column, "%s", formatter.str().c_str());

        elapsed_time(process_uptime, process.uptime);
        mvwprintw(m_process_window, row, time_column, "%s", process_uptime.c_str());
        mvwprintw(m_process_window, row, command_column, "%s", process.command.substr(0, m_process_window->_maxx - command_column).c_str());
    }
}

void NCursesPrinter::build_progress_bar(std::string &bar, double percent)
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

void NCursesPrinter::build_progress_bar(std::string &bar, double actual_value, double total_value, std::string& units)
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
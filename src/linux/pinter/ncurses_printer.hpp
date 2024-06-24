#pragma once

#include <curses.h>
#include <iomanip>
#include <sstream>

#include "sys_info/system_info.hpp"
#include "../../common/conversions.h" // convert_unit
#include "../../utils/utils.h" // precision

// TODO: add descr
class NCursesPrinter
{
    public:
        // TODO: check n
        NCursesPrinter(int n);

        ~NCursesPrinter() { endwin(); }

        void print(SystemInfo &system_info);

    private:
        void print_system(SystemInfo &system_info);

        void print_cpu(SystemInfo &system_info);

        void print_processes(SystemInfo &system_info);

        void build_progress_bar(std::string &bar, double percent, int bar_width);

        void build_progress_bar(std::string &bar, double actual_value, double total_value, int bar_width, std::string& units);

        void resize();

        WINDOW *m_system_window;
        WINDOW *m_cpu_window;
        WINDOW *m_process_window;

        int m_scr_max_y;
        int m_scr_max_x;
        const int m_upper_window_height = 9;
};
#pragma once

#include <iomanip> // std::setprecision
#include <sstream> // std::stringstream

// TODO: add descr
struct UiHelpers
{
    static void build_progress_bar(std::string &bar, double percent)
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

    static void build_progress_bar(std::string &bar, double actual_value, double total_value, const std::string& units)
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
};
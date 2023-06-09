#pragma once

/** @brief Represents load average stats */
struct LoadAverageStats
{
    /** @brief Load average 1 minute */
    double load_average_1;
    /** @brief Load average 5 minutes */
    double load_average_5;
    /** @brief Load average 15 minutes */
    double load_average_15;
};

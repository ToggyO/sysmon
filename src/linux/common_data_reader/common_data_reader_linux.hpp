#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>

#include "sys_info/process.hpp"
#include "../../common/error.h" // check_fs_is_open_or_throw

/** @brief Represents functionality for collecting common Linux specific data */
class CommonDataReaderLinux
{
    public:
        /** @brief Set operational system name
        * @param std::string Reference to result variable
        */
        static void set_os_name(std::string &os_name);

        /** @brief Get system up time
         * @return System up time
         */
        static size_t get_system_uptime();
};
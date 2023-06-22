#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream> // std::stringstream
#include <vector>

#include "sys_info/process.hpp"
#include "../../common/error.h" // check_fs_is_open_or_throw
#include "../filesystem/system_files_reader.hpp"

/** @brief Represents functionality for collecting common Linux specific data */
class CommonDataReaderLinux
{
    public:
        /** @brief Creates new instance of CommonDataReaderLinux
         * @param file_reader Pointer to instance of SystemFilesReader
        */
        explicit CommonDataReaderLinux(SystemFilesReader *file_reader);

        /** @brief Set operational system name
        * @param std::string Reference to result variable
        */
        void set_os_name(std::string &os_name);

        /** @brief Get system up time
         * @return System up time
         */
        size_t get_system_uptime();

    private:
        SystemFilesReader *const m_file_reader;
};
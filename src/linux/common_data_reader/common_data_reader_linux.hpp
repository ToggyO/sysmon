#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream> // std::stringstream
#include <vector>

#include "sys_info/process.hpp"
#include "../../common/error.h" // check_fs_is_open_or_throw
#include "../filesystem/system_files_reader.interface.hpp"

/** @brief Represents functionality for collecting common Linux specific data */
class CommonDataReaderLinux
{
    public:
        /** @brief Creates new instance of CommonDataReaderLinux
         * @param files_reader_ptr Pointer to instance of ISystemFilesReader
        */
        explicit CommonDataReaderLinux(const std::shared_ptr<ISystemFilesReader>& files_reader_ptr);

        /** @brief Set operational system name
        * @param std::string Reference to result variable
        */
        std::string get_os_name();

        /** @brief Get system up time
         * @return System up time
         */
        size_t get_system_uptime();

    private:
        std::weak_ptr<ISystemFilesReader> m_files_reader_ptr;
};

#include "common_data_reader_linux.hpp"
#include "../../common/constants.hpp"

#include "../linux_constants.hpp"

void CommonDataReaderLinux::set_os_name(std::string &os_name)
{
    std::ifstream os_fs(k_os_file_path);
    check_fs_is_open_or_throw(os_fs, k_os_file_path);

    std::string line;
    std::string key;
    std::string value;
    std::istringstream tokenizer;

    while (std::getline(os_fs, line))
    {
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, k_equal_sign_delimeter);
        if (key != k_pretty_name_key) { continue; }

        std::getline(tokenizer, value);
        os_name = value;
        break;
    }
}

size_t CommonDataReaderLinux::get_system_uptime()
{
    std::ifstream uptime_fs(k_uptime_file_path);
    check_fs_is_open_or_throw(uptime_fs, k_uptime_file_path);

    size_t uptime = 0;
    size_t idletime = 0;
    std::string line;

    if (std::getline(uptime_fs, line))
    {
        std::istringstream iss{line};
        iss >> uptime >> idletime;
    }

    return uptime;
}
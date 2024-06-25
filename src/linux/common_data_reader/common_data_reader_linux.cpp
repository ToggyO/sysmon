#include "common_data_reader_linux.hpp"
#include "../../common/constants.hpp" // Constants::k_equal_sign_delimiter

#include "../linux_constants.hpp" // k_uptime_file_path

CommonDataReaderLinux::CommonDataReaderLinux(const std::shared_ptr<ISystemFilesReader>& files_reader_ptr)
    : m_files_reader_ptr{files_reader_ptr}
{}

std::string CommonDataReaderLinux::get_os_name()
{
    auto files_reader = m_files_reader_ptr.lock();
    if (!files_reader)
    {
        throw std::runtime_error("CommonDataReaderLinux::get_os_name - files reader is required");
    }

    std::stringstream ss;
    files_reader->read_etc_os_release(ss);

    std::string line;
    std::string key;
    std::string value;
    std::istringstream tokenizer;

    while (std::getline(ss, line))
    {
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, Constants::k_equal_sign_delimiter);
        if (key != LinuxConstants::k_pretty_name_key) { continue; }

        std::getline(tokenizer, value);
        break;
    }

    return value;
}

size_t CommonDataReaderLinux::get_system_uptime()
{
    auto files_reader = m_files_reader_ptr.lock();
    if (!files_reader)
    {
        throw std::runtime_error("CommonDataReaderLinux::get_system_uptime - files reader is required");
    }

    std::stringstream ss;
    files_reader->read_proc_uptime(ss);

    size_t uptime = 0;
    size_t idle_time = 0;
    std::string line;

    if (std::getline(ss, line))
    {
        std::istringstream iss{line};
        iss >> uptime >> idle_time;
    }

    return uptime;
}

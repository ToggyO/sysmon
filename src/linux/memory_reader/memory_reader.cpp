#include "memory_reader.hpp"

MemoryReader::MemoryReader(const std::shared_ptr<ISystemFilesReader>& files_reader_ptr)
    : m_files_reader_ptr{files_reader_ptr}
{}

void MemoryReader::read(MemoryStats& mem_stats) const
{
    auto files_reader = m_files_reader_ptr.lock();
    if (!files_reader)
    {
        throw std::runtime_error("MemoryReader: files reader is required");
    }

    std::stringstream proc_mem_info;
    files_reader->read_proc_meminfo(proc_mem_info);

    std::string line;
    std::string key;
    std::string value;
    std::istringstream tokenizer;
    size_t value_start = 0;
    size_t value_end = 0;

    unsigned int total_mem = 0;
    unsigned int used_mem = 0;

    while (getline(proc_mem_info, line))
    {
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, Constants::k_colon_delimiter);
        std::getline(tokenizer, value);

        value_start = value.find_first_not_of(' ');
        value_end = value.find_first_of('k');
        if (key == LinuxConstants::k_mem_total)
        {
            total_mem = substr_and_stoi(value, value_start, value_end);
        }

        if (key == LinuxConstants::k_mem_free)
        {
            used_mem = total_mem - substr_and_stoi(value, value_start, value_end);
        }

        if (key == LinuxConstants::k_shmem)
        {
            used_mem += substr_and_stoi(value, value_start, value_end);
        }

        if (key == LinuxConstants::k_buffers  || key == LinuxConstants::k_cached || key == LinuxConstants::k_sreclaimable)
        {
            used_mem -= substr_and_stoi(value, value_start, value_end);
        }
    }

    mem_stats.total_memory = static_cast<double>(total_mem);
    mem_stats.used_memory =  static_cast<double>(used_mem);
}


unsigned int MemoryReader::substr_and_stoi(const std::string &value, const size_t value_start, const size_t value_end)
{
    return std::stoi(value.substr(value_start, value_end));
}

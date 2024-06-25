#pragma once

#include <memory> // std::shared_ptr, std::weak_ptr

#include "sys_info/memory_stats.hpp" // MemoryStats
#include "../sys_info/io_stats.hpp" //
#include "../filesystem/system_files_reader.interface.hpp" // ISystemFilesReader
#include "../linux_constants.hpp" // LinuxConstants::k_mem_total, LinuxConstants::k_mem_free, LinuxConstants::k_shmem, LinuxConstants::k_buffers, LinuxConstants::k_cached, LinuxConstants::k_sreclaimable
#include "../../common/constants.hpp" // Constants::k_colon_delimiter

/** @brief Represents functionality to read and handle RAM */
class MemoryReader
{
public:
    /**
     * Creates new instance of @link MemoryReader::MemoryReader @endlink.
     *
     * @param files_reader_ptr Linux system file reader interface.
     */
    explicit MemoryReader(const std::shared_ptr<ISystemFilesReader>& files_reader_ptr);

    /**
     * Set RAM usage info.
     *
     * @param mem_stats represents system RAM stats.
     */
    void read(MemoryStats& mem_stats) const;

private:
    std::weak_ptr<ISystemFilesReader> m_files_reader_ptr;

    static unsigned int substr_and_stoi(const std::string &value, const size_t value_start, const size_t value_end);
};

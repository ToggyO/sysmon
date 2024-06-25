#pragma once

#include <algorithm>
#include <csignal> // _SC_CLK_TCK
#include <filesystem> // std::filesystem::directory_iterator
#include <sstream> // std::stringstream
#include <vector>

#include "sys_info/process.hpp"
#include "../filesystem/system_files_reader.interface.hpp"
#include "../linux_constants.hpp" // k_proc_directory
#include "../common_data_reader/common_data_reader_linux.hpp"
#include "../../common/constants.hpp" // Constants::k_colon_delimiter

/** @bried Represents functionality for generating descriptions of Linux processes */
class ProcessBuilderLinux
{
public:
    /** @bried Creates new instance of ProcessBuilderLinux
     *
     * @param files_reader_ptr Pointer to instance of Linux system file reader
     *
     * @param CommonDataReaderLinux* Pointer to instance of common Linux specific data collector
     */
    ProcessBuilderLinux(const std::shared_ptr<ISystemFilesReader>& files_reader_ptr, const std::shared_ptr<CommonDataReaderLinux>& common_data_reader_ptr);

    /** @bried Creates new instance of ProcessBuilderLinux
     * @param std::vector<Process> Reference to collection of instances of Process
     */
    void build_processes(std::vector<Process> &);

private:
    /** @brief Calculate process CPU usage by process identifier and set process up time
    * @param Process Process entity
    */
    void calculate_cpu_utilization_and_uptime(Process &);

    /** @brief Set process start command
    * @param Process Process entity
    */
    void calculate_command(Process &);

    /** @brief Set process memory usage
    * @param Process Process entity
    */
    void calculate_memory_usage(Process &);

    /** @brief Set process owner name into provided string parameter
    * @param Process Process entity
    */
    void calculate_process_owner(Process &);

    /** @brief Set process UID
     * @static
     * @see '/proc/<pid>/status' in https://man7.org/linux/man-pages/man5/proc.5.html
     * @param size_t Process identifier
     * @param std::string Reference to result variable
     */
    void set_uid(const size_t &, std::string &);

    /** @bried Linux system file reader interface */
    const std::shared_ptr<ISystemFilesReader> m_file_reader_ptr;
    /** @brief Represents functionality for collecting common Linux specific data */
    const std::shared_ptr<CommonDataReaderLinux> m_common_data_reader_ptr;
};
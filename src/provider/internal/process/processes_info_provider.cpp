#include "processes_info_provider.hpp"
#include <charconv> // std::to_chars, std::chars_format
#include <memory>   // std::unique_ptr
#include <string>   // std::string

#include "../../../utils/utils.h" // elapsed_time

// ProcessInfo
ProcessInfo::ProcessInfo(const Process& process)
    : m_process{process}
{
}

std::string ProcessInfo::get_pid_string() const { return std::to_string(m_process.pid); }

std::string ProcessInfo::get_user() const { return m_process.user; }

std::string ProcessInfo::get_converted_mem_usage_value(BYTE_UNITS to, size_t precision) const
{
    if (precision < 0)
    {
        precision = 0;
    }

    auto converted_value = convert_unit(m_process.memory_usage, to, BYTE_UNITS::KILOBYTES);

    char buffer[32];

    auto formatted_value =
        std::to_chars(buffer, buffer + sizeof(buffer), converted_value, std::chars_format::fixed, precision);
    return std::string(buffer, formatted_value.ptr);
}

std::string ProcessInfo::get_cpu_usage_percent(size_t precision) const
{
    if (precision < 0)
    {
        precision = 0;
    }

    char buffer[32];

    auto formatted_value =
        std::to_chars(buffer, buffer + sizeof(buffer), m_process.cpu_usage * 100, std::chars_format::fixed, precision);
    return std::string(buffer, formatted_value.ptr);
}

std::string ProcessInfo::get_uptime() const
{
    std::string process_uptime;
    elapsed_time(process_uptime, m_process.uptime);
    return process_uptime;
}

std::string ProcessInfo::get_command_slice(size_t start, size_t end) const
{
    return m_process.command.substr(start, end);
}

// ProcessInfoIterator
ProcessInfoIterator::ProcessInfoIterator(const std::vector<Process>& processes_info_collection)
    : m_index{0},
      m_processes_info_collection{processes_info_collection}
{
}

/** @brief Checks whether the iterator has more elements. */
bool ProcessInfoIterator::has_next() const
{
    if (m_processes_info_collection.empty())
    {
        return false;
    }
    return m_index < m_processes_info_collection.size() - 1;
}

/** @brief Returns the next element in the iteration. */
std::unique_ptr<sys_format::IProcessInfo> ProcessInfoIterator::next()
{
    const auto& process = m_processes_info_collection[m_index++];
    return std::make_unique<ProcessInfo>(process);
}

// ProcessesInfoProviderImpl
ProcessesInfoProviderImpl::ProcessesInfoProviderImpl(const SystemInfo* sys_info)
    : m_sys_info_ptr{sys_info}
{
}

std::unique_ptr<sys_format::IProcessInfoIterator> ProcessesInfoProviderImpl::create_iterator() const
{
    return std::make_unique<ProcessInfoIterator>(m_sys_info_ptr->processes);
}

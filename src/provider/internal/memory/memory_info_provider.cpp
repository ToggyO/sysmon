#include "memory_info_provider.hpp"
#include <charconv> // std::to_chars, std::chars_format::fixed

MemoryInfoProviderImpl::MemoryInfoProviderImpl(const SystemInfo* sys_info)
    : m_sys_info_ptr{sys_info}
{
}

double MemoryInfoProviderImpl::get_converted_mem_value(sys_format::RAMTotalOrUsed mem_value_type, BYTE_UNITS to) const
{
    auto val = m_sys_info_ptr->memory_stats.total_memory;
    if (mem_value_type == sys_format::RAMTotalOrUsed::USED)
    {
        val = m_sys_info_ptr->memory_stats.used_memory;
    }

    return convert_unit(val, to, BYTE_UNITS::KILOBYTES);
}

double MemoryInfoProviderImpl::get_converted_swap_value(sys_format::SwapTotalOrFree swap_value_type,
                                                        BYTE_UNITS to) const
{
    auto val = m_sys_info_ptr->memory_stats.swap_total;
    if (swap_value_type == sys_format::SwapTotalOrFree::FREE)
    {
        val = m_sys_info_ptr->memory_stats.swap_free;
    }

    return convert_unit(val, to, BYTE_UNITS::KILOBYTES);
}

std::string MemoryInfoProviderImpl::to_string(const double target, size_t precision) const
{
    if (precision < 0)
    {
        precision = 0;
    }

    char buffer[32];

    auto formatted_value = std::to_chars(buffer, buffer + sizeof(buffer), target, std::chars_format::fixed, precision);
    return std::string(buffer, formatted_value.ptr);
}

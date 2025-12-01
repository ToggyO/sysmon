#include "memory_info_provider.hpp"

MemoryInfoProviderImpl::MemoryInfoProviderImpl(SystemInfo& sys_info)
    : m_sys_info_ref{sys_info}
{
}

double MemoryInfoProviderImpl::get_converted_mem_value(sys_format::RAMTotalOrUsed mem_value_type, BYTE_UNITS to) const
{
    auto sys_info = m_sys_info_ref.get();
    auto val = sys_info.memory_stats.total_memory;
    if (mem_value_type == sys_format::RAMTotalOrUsed::USED)
    {
        val = sys_info.memory_stats.used_memory;
    }

    return convert_unit(val, to, BYTE_UNITS::KILOBYTES);
}

double MemoryInfoProviderImpl::get_converted_swap_value(sys_format::SwapTotalOrFree swap_value_type,
                                                        BYTE_UNITS to) const
{
    auto sys_info = m_sys_info_ref.get();
    auto val = sys_info.memory_stats.swap_total;
    if (swap_value_type == sys_format::SwapTotalOrFree::FREE)
    {
        val = sys_info.memory_stats.swap_free;
    }

    return convert_unit(val, to, BYTE_UNITS::KILOBYTES);
}

MemoryInfoProviderImpl& MemoryInfoProviderImpl::operator=(MemoryInfoProviderImpl&& other) noexcept
{
    if (this != &other)
    {
        m_sys_info_ref = std::move(other.m_sys_info_ref);
    }
    return *this;
}

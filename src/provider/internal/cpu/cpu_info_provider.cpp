#include "cpu_info_provider.hpp"

CpuInfoProviderImpl::CpuInfoProviderImpl(const SystemInfo* sys_info)
    : m_sys_info_ptr{sys_info}
{
}

size_t CpuInfoProviderImpl::get_cpu_cores_count() const { return m_sys_info_ptr->cpu_load_collection.size(); }

const CpuLoad& CpuInfoProviderImpl::get_cpu_core_info(size_t index) const
{
    if (index > m_sys_info_ptr->cpu_load_collection.size() - 1)
    {
        throw std::out_of_range("Invalid cpu core index");
    }
    // reference to stack memory associated with local variable
    return m_sys_info_ptr->cpu_load_collection[index];
}

#include "cpu_info_provider.hpp"

CpuInfoProviderImpl::CpuInfoProviderImpl(SystemInfo& sys_info)
    : m_sys_info_ref{sys_info}
{
}

size_t CpuInfoProviderImpl::get_cpu_cores_count() const
{
    auto sys_info = m_sys_info_ref.get();
    return sys_info.cpu_load_collection.size();
}

const CpuLoad& CpuInfoProviderImpl::get_cpu_core_info(size_t index) const
{
    auto sys_info = m_sys_info_ref.get();
    if (index > sys_info.cpu_load_collection.size() - 1)
    {
        throw std::out_of_range("Invalid cpu core index");
    }
    // reference to stack memory associated with local variable
    return m_sys_info_ref.get().cpu_load_collection[index];
}

CpuInfoProviderImpl& CpuInfoProviderImpl::operator=(CpuInfoProviderImpl&& other) noexcept
{
    if (this != &other)
    {
        m_sys_info_ref = std::move(other.m_sys_info_ref);
    }
    return *this;
}

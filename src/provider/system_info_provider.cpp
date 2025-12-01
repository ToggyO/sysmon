#include "system_info_provider.hpp"
#include "cpu_info_provider.hpp"
#include "memory_info_provider.hpp"
#include "process/processes_info_provider.hpp"

SystemInfoProvider::SystemInfoProvider(const SystemInfo& sys_info)
    : m_sys_info{sys_info},
      m_impl{std::make_unique<Impl>(*this)}
{
}

const sys_format::IMemoryInfoProvider& SystemInfoProvider::get_mem_info_provider() const
{
    return *(m_impl->m_mem_info_provider_impl);
}

const sys_format::ICpuInfoProvider& SystemInfoProvider::get_cpu_info_provider() const
{
    return *(m_impl->m_cpu_info_provider_impl);
}

const sys_format::IProcessesInfoProvider& SystemInfoProvider::get_processes_info_provider() const
{
    return *(m_impl->m_processes_info_provider_impl);
}

/**
 * @brief Internal implementation struct.
 *
 * Stores actual provider implementations.
 */
struct SystemInfoProvider::Impl
{
    Impl(const SystemInfoProvider& sys_info_provider)
        : m_mem_info_provider_impl{std::make_unique<MemoryInfoProviderImpl>(sys_info_provider)},
          m_cpu_info_provider_impl(std::make_unique<CpuInfoProviderImpl>(sys_info_provider)),
          m_processes_info_provider_impl(std::make_unique<ProcessesInfoProvider>(sys_info_provider))
    {
    }

    std::unique_ptr<sys_format::IMemoryInfoProvider> m_mem_info_provider_impl;
    std::unique_ptr<sys_format::ICpuInfoProvider> m_cpu_info_provider_impl;
    std::unique_ptr<sys_format::IProcessesInfoProvider> m_processes_info_provider_impl;
};

#include "system_info_provider.hpp"
#include "./internal/cpu/cpu_info_provider.hpp"           // CpuInfoProviderImpl
#include "./internal/memory/memory_info_provider.hpp"     // MemoryInfoProviderImpl
#include "./internal/process/processes_info_provider.hpp" // ProcessesInfoProviderImpl
#include "sys_info/system_info.hpp"                       // SystemInfo

/**
 * @brief Internal implementation struct.
 *
 * Stores actual provider implementations.
 */
struct SystemInfoProvider::Impl
{
    explicit Impl(const SystemInfo* sys_info)
        : m_mem_info_provider_impl{std::make_unique<MemoryInfoProviderImpl>(sys_info)},
          m_cpu_info_provider_impl(std::make_unique<CpuInfoProviderImpl>(sys_info)),
          m_processes_info_provider_impl(std::make_unique<ProcessesInfoProviderImpl>(sys_info))
    {
    }

    std::unique_ptr<sys_format::IMemoryInfoProvider> m_mem_info_provider_impl;
    std::unique_ptr<sys_format::ICpuInfoProvider> m_cpu_info_provider_impl;
    std::unique_ptr<sys_format::IProcessesInfoProvider> m_processes_info_provider_impl;
};

SystemInfoProvider::SystemInfoProvider(const SystemInfo& sys_info)
    : m_impl{std::make_unique<Impl>(&sys_info)}
{
}

SystemInfoProvider::~SystemInfoProvider() = default;

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

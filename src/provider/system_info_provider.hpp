#pragma once

#include "common/formatting/cpu_info_provider.interface.hpp"       // sys_format::ICpuInfoProvider
#include "common/formatting/procceses_info_provider.interface.hpp" // sys_format::IProcessesInfoProvider
#include "common/formatting/system_info_provider.interface.hpp"    // sys_format::ISystemInfoProvider
#include "sys_info/system_info.hpp"                                // SystemInfo

/**
 * @copydoc sys_format::ISystemInfoProvider.
 *
 * @brief Functionality for preparing system information for display -
 * imlementation.
 */
class SystemInfoProvider : public sys_format::ISystemInfoProvider
{
private:
    /**
     * @brief Opaque implementation (PImpl idiom).
     *
     * Fully hides implementation details and reduces header coupling.
     */
    struct Impl;

public:
    /** @brief Creates an instance of @link
     * SystemInfoProvider::SystemInfoProvider @endlink */
    SystemInfoProvider(const SystemInfo&);

    SystemInfoProvider(const SystemInfoProvider&) = delete;
    SystemInfoProvider(SystemInfoProvider&&) = default;

    ~SystemInfoProvider() override;

    /**
     * @copydoc sys_format::ISystemInfoProvider::get_mem_info_provider()
     *
     * @brief Returns a reference to a RAM stats provider.
     */
    const sys_format::IMemoryInfoProvider& get_mem_info_provider() const override;

    /**
     * @copydoc sys_format::ISystemInfoProvider::get_cpu_info_provider()
     *
     * @brief Returns a reference to a CPU stats provider.
     */
    const sys_format::ICpuInfoProvider& get_cpu_info_provider() const override;

    /**
     * @copydoc
     * sys_format::IProcessesInfoProvider::get_processes_info_provider()
     *
     * @brief Returns a reference to a CPU stats formatter.
     */
    const sys_format::IProcessesInfoProvider& get_processes_info_provider() const override;

    SystemInfoProvider& operator=(const SystemInfoProvider&) = delete;
    SystemInfoProvider& operator=(SystemInfoProvider&&) noexcept = default;

private:
    std::unique_ptr<Impl> m_impl;
};

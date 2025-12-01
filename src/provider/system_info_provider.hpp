#pragma once

#include "../common/formatting/system_info_provider.interface.hpp"
#include "common/formatting/cpu_info_provider.interface.hpp"
#include "common/formatting/procceses_info_provider.interface.hpp"
#include "sys_info/system_info.hpp"

// TODO: в импортах указать, что импортируем
// TODO: разделить реализации на cpp/hpp или поставлять либой?
/**
 * @copydoc sys_format::ISystemInfoProvider.
 *
 * @brief Functionality for preparing system information for display - imlementation.
 */
class SystemInfoProvider : public sys_format::ISystemInfoProvider
{
public:
    /** @brief Creates an instance of @link SystemInfoProvider::SystemInfoProvider @endlink */
    SystemInfoProvider(const SystemInfo&);

    SystemInfoProvider(const SystemInfoProvider&) = delete;
    SystemInfoProvider(SystemInfoProvider&&) = default;

    ~SystemInfoProvider() = default;

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
     * @copydoc sys_format::IProcessesInfoProvider::get_processes_info_provider()
     *
     * @brief Returns a reference to a CPU stats formatter.
     */
    const sys_format::IProcessesInfoProvider& get_processes_info_provider() const override;

    SystemInfoProvider& operator=(const SystemInfoProvider&) = delete;
    SystemInfoProvider& operator=(SystemInfoProvider&&) noexcept = default;

private:
    /**
     * @brief Opaque implementation (PImpl idiom).
     *
     * Fully hides implementation details and reduces header coupling.
     */
    struct Impl;

    std::unique_ptr<Impl> m_impl;

    const SystemInfo& m_sys_info;
};

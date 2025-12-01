#pragma once

#include "sys_info/system_info.hpp"
#include <functional>

#include "../../../common/formatting/cpu_info_provider.interface.hpp"

/** @brief CPU stats provider. */
class CpuInfoProviderImpl : public sys_format::ICpuInfoProvider
{
public:
    /** @brief Creates new instance on @link CpuInfoProviderImpl @endlink. */
    explicit CpuInfoProviderImpl(SystemInfo&);

    CpuInfoProviderImpl(const CpuInfoProviderImpl&) = delete;
    CpuInfoProviderImpl(CpuInfoProviderImpl&&) noexcept = default;

    ~CpuInfoProviderImpl() = default;

    /**
     * @copydoc sys_format::ICpuInfoProvider::get_cpu_cores_count()
     *
     * @brief Get the number of cpu cores.
     */
    size_t get_cpu_cores_count() const override;

    /**
     * @copydoc sys_format::ICpuInfoProvider::get_cpu_core_info(size_t)
     *
     * @brief Get cpu core brief info.
     */
    virtual const CpuLoad& get_cpu_core_info(size_t index) const override;

    CpuInfoProviderImpl& operator=(const CpuInfoProviderImpl&) = delete;
    CpuInfoProviderImpl& operator=(CpuInfoProviderImpl&&) noexcept;

private:
    std::reference_wrapper<SystemInfo> m_sys_info_ref;
};

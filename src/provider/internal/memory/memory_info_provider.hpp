#pragma once

#include "../../../common/formatting/memory_info_provider.interface.hpp"
#include "sys_info/system_info.hpp" // SystemInfo

/** @brief RAM stats provider. */
class MemoryInfoProviderImpl : public sys_format::IMemoryInfoProvider
{
public:
    /** @brief Creates new instance of @link MemoryInfoProviderImpl @endlink. */
    explicit MemoryInfoProviderImpl(const SystemInfo*);

    MemoryInfoProviderImpl(const MemoryInfoProviderImpl&) = delete;
    MemoryInfoProviderImpl(MemoryInfoProviderImpl&&) noexcept = default;

    ~MemoryInfoProviderImpl() = default;

    /**
     * @copydoc sys_format::IMemoryInfoProvider::get_converted_mem_value(sys_format::RAMTotalOrUsed, BYTE_UNITS)
     *
     * @brief Returns an amount of total or used RAM, converted into necessary format.
     */
    virtual double get_converted_mem_value(sys_format::RAMTotalOrUsed mem_value_type, BYTE_UNITS to) const override;

    /**
     * @copydoc sys_format::IMemoryInfoProvider::get_converted_swap_value(sys_format::SwapTotalOrFree, BYTE_UNITS)
     *
     * @brief Returns an amount of total or free swap, converted into necessary format.
     */
    virtual double get_converted_swap_value(sys_format::SwapTotalOrFree swap_value_type, BYTE_UNITS to) const override;

    /**
     * @copydoc sys_format::IMemoryInfoProvider::to_string(const double target, size_t precision)
     *
     * @brief Coverts target num into string.
     */
    virtual std::string to_string(const double target, size_t precision) const override;

    MemoryInfoProviderImpl& operator=(const MemoryInfoProviderImpl&) = delete;
    MemoryInfoProviderImpl& operator=(MemoryInfoProviderImpl&&) noexcept = default;

private:
    const SystemInfo* m_sys_info_ptr;
};

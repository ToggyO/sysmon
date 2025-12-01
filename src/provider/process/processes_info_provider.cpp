#pragma once

#include "../../common/formatting/procceses_info_provider.interface.hpp"
#include "../system_info_provider.hpp"
#include "sys_info/process.hpp"

// TODO: add descr
class ProcessInfo : public sys_format::IProcessInfo
{
};

// TODO: add descr
class ProcessInfoIterator : public sys_format::IProcessInfoIterator
{
public:
    explicit ProcessInfoIterator(const std::vector<Process>& processes_info_collection)
        : m_processes_info_collection{processes_info_collection}
    {
    }

    /** @brief Checks whether the iterator has more elements. */
    virtual bool has_next() const = 0;

    /** @brief Returns the next element in the iteration. */
    virtual IProcessInfo next() = 0;

private:
    const std::vector<Process>& m_processes_info_collection;
};

/** @brief System processes info provider. */
class ProcessesInfoProviderImpl : public sys_format::IProcessesInfoProvider
{
public:
    /** @brief Creates new instance of @link ProcessesInfoProviderImpl @endlink. */
    explicit ProcessesInfoProviderImpl(const SystemInfoProvider& parent)
        : m_parent{parent}
    {
    }

    /**
     * @copydoc sys_format::IProcessInfoIterator::create_iterator()
     *
     * @brief @brief Creates a new iterator for traversing system process information.
     */
    std::unique_ptr<sys_format::IProcessInfoIterator> create_iterator() const override
    {
        return std : make_unique<ProcessInfoIterator>(m_parent.sys_info)
    }

private:
    const SystemInfoProvider& m_parent;
};

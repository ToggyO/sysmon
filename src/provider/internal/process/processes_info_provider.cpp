#include "processes_info_provider.hpp"

// ProcessInfoIterator
ProcessInfoIterator::ProcessInfoIterator(const std::vector<Process>& processes_info_collection)
    : m_index{0},
      m_processes_info_collection{processes_info_collection}
{
}

/** @brief Checks whether the iterator has more elements. */
bool ProcessInfoIterator::has_next() const { return m_index < m_processes_info_collection.size() - 1; }

/** @brief Returns the next element in the iteration. */
sys_format::IProcessInfo ProcessInfoIterator::next()
{
    const auto& process = m_processes_info_collection[m_index++];
    return ProcessInfo(process);
}

// ProcessesInfoProviderImpl
ProcessesInfoProviderImpl::ProcessesInfoProviderImpl(SystemInfo& sys_info)
    : m_sys_info_ref{sys_format}
{
}

std::unique_ptr<sys_format::IProcessInfoIterator> ProcessesInfoProviderImpl::create_iterator() const
{
    return std::make_unique<ProcessInfoIterator>(m_sys_info_ref.get().cpu_load_collection);
}

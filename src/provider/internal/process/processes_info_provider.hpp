#pragma once

#include "../../../common/formatting/procceses_info_provider.interface.hpp" // sys_format::IProcessesInfoProvider, sys_format::IProcessInfoIterator, sys_format::IProcessInfo
#include "sys_info/process.hpp"     // Process
#include "sys_info/system_info.hpp" // SystemInfo
#include <string>

// TODO: copy + move
/** @brief Provides access to system process information. */
class ProcessInfo : public sys_format::IProcessInfo
{
public:
    /** @brief Creates new instance of @link ProcessInfo::ProcessInfo @endlink. */
    explicit ProcessInfo(const Process&);

    /**
     * @copydoc sys_format::IProcessInfo::get_pid()
     *
     * @brief Returns the process stringified identifier (PID).
     *
     * @return Process ID as an unsigned integer.
     */
    virtual std::string get_pid_string() const override;

    /**
     * @copydoc sys_format::IProcessInfo::get_user()
     *
     * @brief Returns the user name that owns the process.
     *
     * The returned value is a string representation of the user
     * associated with the process. Implementations typically
     * translate UID to a username string.
     *
     * @return A copy of the user name string.
     */
    virtual std::string get_user() const override;

    /**
     * @copydoc sys_format::IProcessInfo::get_converted_mem_usage_value(BYTE_UNITS)
     *
     * @brief Converts the process memory usage into the requested unit and stringifies it.
     *
     * @param to Target memory units (bytes, KB, MB, etc.).
     *
     * @param precision Result value precision.
     *
     * @return Memory usage expressed in the requested unit.
     */
    virtual std::string get_converted_mem_usage_value(BYTE_UNITS, size_t) const override;

    /**
     * @copydoc sys_format::IProcessInfo::get_cpu_usage_percent(bool)
     *
     * @brief Returns CPU usage percentage and stringifies it.
     *
     * @return CPU usage percentage string value.
     */
    virtual std::string get_cpu_usage_percent(size_t) const override;

    /**
     * @copydoc sys_format::IProcessInfo::get_uptime()
     *
     * @brief Returns the process uptime in a formatted human-readable form.
     *
     * Typical output formats include "1h 20m 15s" or similar.
     *
     * @return A formatted uptime string.
     */
    virtual std::string get_uptime() const override;

    /**
     * @copydoc sys_format::IProcessInfo::get_formatted_command(size_t, size_t)
     *
     * @brief Returns a slice from command used to launch the process.
     *
     * The returned value typically includes the executable name and
     * its command-line arguments, formatted into a single string.
     *
     * @return A formatted process command string.
     */
    virtual std::string get_command_slice(size_t, size_t) const override;

private:
    const Process& m_process;
};

/** @brief Iterator for traversing system process information entries. */
class ProcessInfoIterator : public sys_format::IProcessInfoIterator
{
public:
    explicit ProcessInfoIterator(const std::vector<Process>&);

    ProcessInfoIterator(const ProcessInfoIterator&) = delete;
    ProcessInfoIterator(ProcessInfoIterator&&) = delete;

    ~ProcessInfoIterator() = default;

    /** @brief Checks whether the iterator has more elements. */
    virtual bool has_next() const override;

    /** @brief Returns the next element in the iteration. */
    virtual std::unique_ptr<sys_format::IProcessInfo> next() override;

    ProcessInfoIterator& operator=(const ProcessInfoIterator&) = delete;
    ProcessInfoIterator& operator=(ProcessInfoIterator&&) = delete;

private:
    size_t m_index;
    const std::vector<Process>& m_processes_info_collection;
};

/** @brief System processes info provider. */
class ProcessesInfoProviderImpl : public sys_format::IProcessesInfoProvider
{
public:
    /** @brief Creates new instance of @link ProcessesInfoProviderImpl @endlink. */
    explicit ProcessesInfoProviderImpl(const SystemInfo*);

    ProcessesInfoProviderImpl(const ProcessesInfoProviderImpl&) = delete;
    ProcessesInfoProviderImpl(ProcessesInfoProviderImpl&&) = default;

    ~ProcessesInfoProviderImpl() = default;

    /**
     * @copydoc sys_format::IProcessInfoIterator::create_iterator()
     *
     * @brief @brief Creates a new iterator for traversing system process information.
     */
    virtual std::unique_ptr<sys_format::IProcessInfoIterator> create_iterator() const override;

    ProcessesInfoProviderImpl& operator=(const ProcessesInfoProviderImpl&) = delete;
    ProcessesInfoProviderImpl& operator=(ProcessesInfoProviderImpl&&) = default;

private:
    const SystemInfo* m_sys_info_ptr;
};

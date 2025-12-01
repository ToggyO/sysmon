#pragma once

#include "../../../common/formatting/procceses_info_provider.interface.hpp" // sys_format::IProcessesInfoProvider, sys_format::IProcessInfoIterator, sys_format::IProcessInfo
#include "sys_info/process.hpp" // SystemInfo
#include <functional>           // std::reference_wrapper

// TODO: add descr
class ProcessInfo : public sys_format::IProcessInfo
{
public:
    explicit ProcessInfo(const Process&);

    /**
     * @brief Returns the process identifier (PID).
     *
     * @return Process ID as an unsigned integer.
     */
    virtual size_t get_pid() const = 0;

    /**
     * @brief Returns the user name that owns the process.
     *
     * The returned value is a string representation of the user
     * associated with the process. Implementations typically
     * translate UID to a username string.
     *
     * @return A copy of the user name string.
     */
    virtual std::string get_user() const = 0;

    /**
     * @brief Converts the process memory usage into the requested unit.
     *
     * @param to Target memory units (bytes, KB, MB, etc.).
     * @return Memory usage expressed in the requested unit.
     */
    virtual double get_converted_mem_usage_value(BYTE_UNITS to) const = 0;

    /**
     * @brief Returns CPU usage percentage.
     *
     * @return CPU usage percentage value.
     */
    virtual double get_cpu_usage_percent(bool round_value) const = 0;

    /**
     * @brief Returns the process uptime in a formatted human-readable form.
     *
     * Typical output formats include "1h 20m 15s" or similar.
     *
     * @return A formatted uptime string.
     */
    virtual std::string get_uptime() const = 0;

    /**
     * @brief Returns the full command used to launch the process.
     *
     * The returned value typically includes the executable name and
     * its command-line arguments, formatted into a single string.
     *
     * @return A formatted process command string.
     */
    virtual std::string get_formatted_command() const = 0;
};

// TODO: add descr
class ProcessInfoIterator : public sys_format::IProcessInfoIterator
{
public:
    explicit ProcessInfoIterator(const std::vector<Process>&);

    /** @brief Checks whether the iterator has more elements. */
    virtual bool has_next() const override;

    /** @brief Returns the next element in the iteration. */
    virtual IProcessInfo next() override;

private:
    size_t m_index;
    const std::vector<Process>& m_processes_info_collection;
};

/** @brief System processes info provider. */
class ProcessesInfoProviderImpl : public sys_format::IProcessesInfoProvider
{
public:
    /** @brief Creates new instance of @link ProcessesInfoProviderImpl @endlink. */
    explicit ProcessesInfoProviderImpl(SystemInfo&);

    /**
     * @copydoc sys_format::IProcessInfoIterator::create_iterator()
     *
     * @brief @brief Creates a new iterator for traversing system process information.
     */
    std::unique_ptr<sys_format::IProcessInfoIterator> create_iterator() const override;

private:
    std::reference_wrapper<SystemInfo> m_sys_info_ref;
};

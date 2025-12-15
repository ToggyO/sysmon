#pragma once

#include "../conversions.h" // BYTE_UNITS
#include <memory>           // std::unique_ptr
#include <string>           // std::string

namespace sys_format
{
    /** @brief Provides access to system process information. */
    struct IProcessInfo
    {
        /** @brief Virtual dctor. */
        virtual ~IProcessInfo() = default;

        /**
         * @brief Returns the process stringified identifier (PID).
         *
         * @return Process ID as an unsigned integer.
         */
        virtual std::string get_pid_string() const = 0;

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
         * @brief Converts the process memory usage into the requested unit and stringifies it.
         *
         * @param to Target memory units (bytes, KB, MB, etc.).
         *
         * @param precision Result value precision.
         *
         * @return Memory usage expressed in the requested unit.
         */
        virtual std::string get_converted_mem_usage_value(BYTE_UNITS to, size_t precision) const = 0;

        /**
         * @brief Returns CPU usage percentage and stringifies it.
         *
         * @return CPU usage percentage string value.
         */
        virtual std::string get_cpu_usage_percent(size_t precision) const = 0;

        /**
         * @brief Returns the process uptime in a formatted human-readable form.
         *
         * Typical output formats include "1h 20m 15s" or similar.
         *
         * @return A formatted uptime string.
         */
        virtual std::string get_uptime() const = 0;

        /**
         * @brief Returns a slice from command used to launch the process.
         *
         * The returned value typically includes the executable name and
         * its command-line arguments, formatted into a single string.
         *
         * @return A formatted process command string.
         */
        virtual std::string get_command_slice(size_t, size_t) const = 0;
    };

    /**
     * @brief Iterator for traversing system process information entries.
     *
     * Provides sequential access to a collection of process information objects.
     * Each iterator instance typically represents a snapshot of the current
     * system state at the moment of iterator creation, unless the concrete
     * implementation specifies otherwise.
     *
     * The typical usage pattern:
     * @code
     * auto it = provider.create_iterator();
     * while (it->has_next()) {
     *     std::unique_ptr<IProcessInfo> info_ptr = it->next();
     *     // process info...
     * }
     * @endcode
     */
    struct IProcessInfoIterator
    {
        /** @brief Virtual dctor. */
        virtual ~IProcessInfoIterator() = default;

        /** @brief Checks whether the iterator has more elements. */
        virtual bool has_next() const = 0;

        /** @brief Returns the next element in the iteration. */
        virtual std::unique_ptr<IProcessInfo> next() = 0;
    };

    /** @brief System processes info provider. */
    struct IProcessesInfoProvider
    {
        /** @brief Virtual dctor. */
        virtual ~IProcessesInfoProvider() = default;

        /**
         * @brief Creates a new iterator for traversing system process information.
         *
         * This method constructs and returns a new iterator that provides
         * sequential access to the collection of currently available system
         * processes. Each call produces a separate iterator instance whose
         * lifetime is owned by the caller.
         *
         * @return A `std::unique_ptr` managing a newly created instance of
         *         @link IProcessInfoIterator @endlink.
         */
        virtual std::unique_ptr<IProcessInfoIterator> create_iterator() const = 0;
    };
}; // namespace sys_format

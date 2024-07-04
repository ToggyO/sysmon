#pragma once

#include <optional>
#include <cstddef>
#include <string_view>

/** @brief Set of file descriptors, referenced to most important linux processes data files. */
struct ProcessFdsSet
{
    /** @brief /proc/<pid>/stat file descriptor. */
    std::optional<int> proc_stat_fd;
    /** @brief /proc/<pid>/cmdl file descriptor. */
    std::optional<int> proc_cmd_fd;
    /** @brief /proc/<pid>/status file descriptor. */
    std::optional<int> proc_status_fd;
};

/** @brief File descriptors cache interface */
struct IFileDescriptorsCache
{
    /**
     * @brief Retrieves from the cache a file descriptor by it's source file path.
     *
     * @param file_path path to source file.
     *
     * @return instance if @link std::optional{int} @endlink
     */
    virtual std::optional<int> get_by_file_path(std::string_view file_path) const = 0;

    /**
     * @brief Retrieves from the cache a set of file descriptors by a process identifier.
     *
     * @param pid a process identifier.
     *
     * @return instance if @link std::optional{ProcessFdsSet} @endlink
     */
    virtual std::optional<ProcessFdsSet> get_by_pid(size_t pid) const = 0;

    /**
     * @brief Saves to the cache a file descriptor by it's source file path.
     *
     * @param file_path path to source file.
     *
     * @param fd file descriptor.
     */
    virtual void save_by_file_path(std::string_view file_path, int fd) = 0;

    /**
     * @brief Saves to the cache a set of file descriptors by process identifier.
     *
     * @param pid process identifier.
     *
     * @param fds set of file descriptors, referenced to most important linux processes data files.
     */
    virtual void save_by_pid(size_t pid, ProcessFdsSet fds) = 0;
};

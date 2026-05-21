#pragma once

#include <string>
#include <unordered_map>

#include "file_descriptors_cache.interface.hpp"

/** @brief File descriptors cache implementation @link IFileDescriptorsCache @endlink */
class FileDescriptorsCache final : public IFileDescriptorsCache
{
public:
    FileDescriptorsCache();

    /**
     * @brief Retrieves from the cache a file descriptor by it's source file path.
     *
     * @param file_path path to source file.
     *
     * @return instance if @link std::optional{int} @endlink
     */
    std::optional<int> get_by_file_path(std::string_view file_path) const override;

    /**
     * @brief Retrieves from the cache a file descriptor by a process identifier.
     *
     * @param pid a process identifier.
     *
     * @return instance if @link std::optional{int} @endlink
     */
    std::optional<ProcessFdsSet> get_by_pid(size_t pid) const override;

    /**
     * @brief Saves to the cache a file descriptor by it's source file path.
     *
     * @param file_path path to source file.
     *
     * @param fd file descriptor.
     */
    void save_by_file_path(std::string_view file_path, int fd) override;

    /**
     * @brief Saves to the cache a file descriptor by process identifier.
     *
     * @param pid process identifier.
     *
     * @param fd file descriptor.
     */
    void save_by_pid(size_t pid, ProcessFdsSet fds) override;

private:
    std::unordered_map<std::string, int> m_fp_cache;
    std::unordered_map<size_t, ProcessFdsSet> m_pid_cache;
};
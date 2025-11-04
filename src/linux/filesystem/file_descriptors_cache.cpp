#include "file_descriptors_cache.hpp"

FileDescriptorsCache::FileDescriptorsCache()
    : m_fp_cache{}, m_pid_cache{}
{}

std::optional<int> FileDescriptorsCache::get_by_file_path(std::string_view file_path) const
{
    auto key = std::string(file_path.data());
    if (auto search = m_fp_cache.find(key); search != m_fp_cache.end())
    {
        return search->second;
    }
    return {};
}

std::optional<ProcessFdsSet> FileDescriptorsCache::get_by_pid(size_t pid) const
{
    if (auto search = m_pid_cache.find(pid); search != m_pid_cache.end())
    {
        return search->second;
    }
    return {};
}

void FileDescriptorsCache::save_by_file_path(std::string_view file_path, int fd)
{
    m_fp_cache.insert({std::string(file_path.data()), fd});
}

void FileDescriptorsCache::save_by_pid(size_t pid, ProcessFdsSet fds)
{
    m_pid_cache.insert({pid, fds});
}
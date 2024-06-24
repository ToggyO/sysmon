#pragma once

#include <chrono> // std::chrono::duration
#include <fcntl.h> // fcntl, F_GETFL, F_SETFL, O_NONBLOCK
#include <memory> // std::shared_ptr, std::weak_ptr
#include <stdexcept> // std::runtime_error
#include <sys/epoll.h> // epoll_create1
#include <thread> // this_thread
#include <unistd.h> // close

#include "system_file_descriptors_handler.interface.hpp"
#include "poller.interface.hpp"

// TODO: add descr
// TODO: copy/move
class SystemPoller : public IPoller
{
public:
    SystemPoller(const std::shared_ptr<ISystemFileDescriptorsHandler>& handler);

    void poll(size_t timeout_ms = 1000) override;

    void update_fds(std::vector<int>&& fds) override;

    void set_non_blocking(int fd) override;

private:
    std::vector<int> m_fds;
    std::weak_ptr<ISystemFileDescriptorsHandler> m_handler_ptr;
};

#include "linux_system_poller.hpp"

SystemPoller::SystemPoller(const std::shared_ptr<ISystemFileDescriptorsHandler>& handler_ptr)
    : m_handler_ptr{handler_ptr}
{}

void SystemPoller::poll(size_t timeout_ms)
{
    auto timeout = std::chrono::milliseconds(timeout_ms);

    auto handler = m_handler_ptr.lock();
    if (!handler)
    {
        throw std::runtime_error("Epoll events handler is required");
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        throw std::runtime_error("epoll_create1");
    }

    struct epoll_event ev;
    struct epoll_event events[m_fds.size()];

    ev.events = EPOLLIN & EPOLLET;

    for (const auto& fd : m_fds)
    {
        set_non_blocking(fd); // TODO: check error on alreade non blocking fd

        ev.data.fd = fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
        {
            throw std::runtime_error("epoll_ctl");
            close(fd);
            close(epoll_fd);
        }
    }

    while (true)
    {
        int nfds = epoll_wait(epoll_fd, events, m_fds.size(), timeout_ms);
        if (nfds == -1)
        {
            throw std::runtime_error("epoll_wait");
        }

        for (int i = 0; i < nfds; ++i)
        {
            if (!(events[i].events & EPOLLIN))
            {
                continue;
            }

            int fd = events[i].data.fd;
            handler->handle(fd);
        }

        std::this_thread::sleep_for(timeout);
    }

    for (int fd : m_fds)
    {
        close(fd);
    }
    close(epoll_fd);
}

void SystemPoller::update_fds(std::vector<int>&& fds)
{
    m_fds = std::move(fds);
}

void SystemPoller::set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        throw std::runtime_error("fcntl"); // TODO: EXCEPTION
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        throw std::runtime_error("fcntl"); // TODO: EXCEPTION
    }
}




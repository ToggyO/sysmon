#pragma once

#include <vector>

struct IPoller
{
    virtual void poll(size_t timeout_ms = 1000) = 0;

    virtual void update_fds(std::vector<int>&& fds) = 0;

    virtual void set_non_blocking(int fd) = 0;
};
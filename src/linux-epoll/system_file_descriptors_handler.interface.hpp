#pragma once

#include <vector>

// TODO: add descr
struct ISystemFileDescriptorsHandler // TODO: это не epoll events handler, а маппер для иденцификации дескриптора, как ситменый файл
{
    virtual int handle(int fd) = 0;
};

#include "system_file_descriptors_handler.interface.hpp"

class LinuxSystemFileDescriptorsHandler : public ISystemFileDescriptorsHandler
{
public:
    // explicit LinuxSystemFileDescriptorsHandler(std::vector<int>&& fds);

    int handle(int fd) override;

private:
    std::vector<int> m_fds;
};

#include "sys_info/process.hpp"

Process::Process(size_t process_id) : pid{process_id}
{}

bool Process::operator<(const Process & a) const
{
    return cpu_usage < a.cpu_usage;
}


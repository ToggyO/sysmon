#pragma once

#include <cstring> // size_t
#include <string>

/** @brief Represent ondformation about system process */
class Process
{
    public:
        /** @brief Process identifier */
        size_t pid;
        /** @brief Process cpu usage */
        double cpu_usage;
        /** @brief The filename of the process executable, in parentheses */
        std::string command;
        /** @brief RAM count is used by Process */
        double memory_usage;
        /** @brief Process owner */
        std::string user;
        /** @brief Shows how long the process */
        size_t uptime;

        /** @brief Creates new instance of Process
         * @param process_id
         */
        explicit Process(size_t process_id);

        /** @brief Overrides '\<' operator for Process */
        bool operator<(const Process &) const;
};

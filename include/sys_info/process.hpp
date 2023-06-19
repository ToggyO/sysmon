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

    private:
        /** @brief Calculate process CPU usage by process identifier
         * @param size_t Process identifier
         */
        void calculate_cpu_utilization();

        /** @brief Set process start command
        * @param size_t Process identifier
        */
        void calculate_command();

        /** @brief Set process memory usage
        * @param size_t Process identifier
        */
        void calculate_memory_usage();

        /** @brief Set process owner name into provided string parameter
        * @param size_t Process identifier
        */
        void calculate_process_owner();

        /** @brief Set process up time
        * @param size_t Process identifier
        */
        void calculate_process_uptime();

        /** @brief Set process UID
         * @static
         * @see '/proc/[pid]/status' in https://man7.org/linux/man-pages/man5/proc.5.html
         * @param size_t Process identifier
         * @param std::string Reference to result variable
         */
        static void set_uid(size_t, std::string &);
};

#pragma once

#include <cstring> // size_t
#include <string>

/** @brief Represent ondformation about system process */
class Process
{
    public:
        /** @brief Creates new instance of Process
         * @param process_id
         */
        explicit Process(size_t process_id) : pid{process_id}
        {
            set_cpu_utilization(process_id);
            set_command(process_id);
            set_memory_usage(process_id);
            set_process_owner(process_id);
            set_process_uptime(process_id);
        }

        /** @brief Calculate process CPU usage by process identifier
         * @param size_t Process identifier
         */
        void set_cpu_utilization(size_t);

        /** @brief Set process start command
        * @param size_t Process identifier
        */
        void set_command(size_t);

        /** @brief Set process memory usage
        * @param size_t Process identifier
        */
        void set_memory_usage(size_t);

        /** @brief Set process owner name into provided string parameter
        * @param size_t Process identifier
        */
        void set_process_owner(size_t);

        /** @brief Set process up time
        * @param size_t Process identifier
        */
        void set_process_uptime(size_t);

        /** @brief Set process UID
         * @static
         * @see '/proc/[pid]/status' in https://man7.org/linux/man-pages/man5/proc.5.html
         * @param size_t Process identifier
         * @param std::string Reference to result variable
         */
        static void set_uid(size_t, std::string &);

        /** @brief Overrides '\<' operator for Process */
        bool operator<(const Process &) const;

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

    private:
        bool is_cpu_utilization_set = false;
        bool is_command_set = false;
        bool is_memory_usage_set = false;
        bool is_user_set = false;
        bool is_uptime_set = false;
};

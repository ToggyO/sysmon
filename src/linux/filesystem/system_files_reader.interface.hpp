#pragma once

#include <iostream>
#include <sstream> // std::stringstream

/** @bried Linux system file reader interface */
class ISystemFilesReader
{
public:
    // ВОПРОС: будет ли вызван деструктор наследника, если виртуальный деструктор базового класса реализован как default
    /** @bried Destroys current instance of ISystemFilesReader */
    virtual ~ISystemFilesReader() = default;

    /** @brief Read data from '/proc/stat'
     * @param std::stringstream Reference to result variable
    */
    virtual void read_proc_stat(std::stringstream &) = 0;

    /** @brief Read data from '/proc/<pid>/stat'
     * @param std::stringstream Reference to result variable
     * @param size_t Process identifier @constant
    */
    virtual void read_process_stat(std::stringstream &, const size_t &) = 0;

    /** @brief Read data from '/proc/<pid>/cmdline'
     * @param std::stringstream Reference to result variable
     * @param size_t Process identifier @constant
    */
    virtual void read_process_cmdline(std::stringstream &, const size_t &) = 0;

    /** @brief Read data from '/etc/passwd'
     * @param std::stringstream Reference to result variable
     * @param size_t Process identifier @constant
    */
    virtual void read_process_etc_passwd(std::stringstream &, const size_t &) = 0;

    /** @brief Read data from '/proc/<pid>/status'
     * @param std::stringstream Reference to result variable
     * @param size_t Process identifier @constant
    */
    virtual void read_process_status(std::stringstream &, const size_t &) = 0;

    /** @brief Read data from '/etc/os-release'
     * @param std::stringstream Reference to result variable
    */
    virtual void read_etc_os_release(std::stringstream &) = 0;

    /** @brief Read data from '/proc/uptime'
     * @param std::stringstream Reference to result variable
    */
    virtual void read_proc_uptime(std::stringstream &) = 0;

    /** @brief Read data from '/proc/meminfo'
     * @param std::stringstream Reference to result variable
    */
    virtual void read_proc_meminfo(std::stringstream &) = 0;
};

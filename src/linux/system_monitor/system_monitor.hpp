#pragma once

#include <algorithm>

#include "sys_info/printer.interface.hpp"
#include "sys_info/system_info.hpp"
#include "../sys_info/cpu_stats.hpp"
#include "../cpu_reader/cpu_reader_linux.hpp"
#include "../filesystem/system_files_reader.interface.hpp"
#include "../common_data_reader/common_data_reader_linux.hpp"
#include "../process_builder/proccess_builder_linux.hpp"
#include "../memory_reader/memory_reader.hpp"
//#include "../printer/ncurses_printer.hpp" TODO: check

/** @brief System monitoring information collector for Linux */
class SystemMonitor
{
public:
    /** @brief Creates new instance of SystemMonitor
     *
     * @param files_reader_ptr Pointer to instance of Linux system file reader
     *
     * @param printer Pointer to instance of system info printer.
     */
   explicit SystemMonitor(std::shared_ptr<ISystemFilesReader>& files_reader_ptr, std::shared_ptr<IPrinter>& printer_ptr)
        : m_cpu_reader(CpuReaderLinux(files_reader_ptr)),
        m_mem_reader{MemoryReader(files_reader_ptr)},
        m_common_data_reader{std::make_shared<CommonDataReaderLinux>(files_reader_ptr)},
        m_process_builder(ProcessBuilderLinux(files_reader_ptr, m_common_data_reader)),
        m_printer_ptr{printer_ptr}
    {}

    // TODO: add descr
    int run(const volatile sig_atomic_t& stop)
    {
        SystemInfo system_info{};
//        NCursesPrinter p(10); // TODO: вынети в интейфейс
    //    Printer p;
        auto printer = m_printer_ptr.lock();
        if (!printer)
        {
            throw std::runtime_error("SystemMonitor: info printer is required");
        }

//        printer->print(system_info);

        // TODO: добавить сбор initial value по CPU
        try
        {
            int iter = 0; // TODO: remove
            while (!stop)
            {
                std::cout << "Stopping sysmon iteration count: " << iter << std::endl; // TODO: remove
                collect(system_info);
//                printer->print(system_info); // TODO: поправить вывод RAM, Load average не печататется!!! // TODO: тут сделать  кравсиво проверку
                std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // TODO: to settings
                iter++; // TODO: remove
            }

            std::cout << "Stopping sysmon. Stop code: " << stop << std::endl; // TODO: remove
        }
        catch (std::exception& e)
        {
            std::cerr << "Sysmon exited with error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Sysmon successfully exited" << std::endl; // TODO: remove
        return EXIT_SUCCESS;
    }

private:
    /** @brief Collects system information and populates SystemInfo with it..
     * @param system_info - struct holds system monitoring information.
     */
    void collect(SystemInfo &system_info)
    {
        // TODO: распараллелить запись
        if (!system_info.is_os_name_set())
        {
            system_info.set_os_name(collect_os_name());
        }
        collect_load(system_info);
        collect_cpu(system_info);
        collect_memory(system_info);
        collect_disks_usage(system_info);
        collect_processes_info(system_info);
        collect_uptime(system_info);
    }

    /** @brief Collects information about operating system name and populates SystemInfo with it.
     * @param system_info - struct holds system monitoring information.
     */
    std::string collect_os_name();

    /** @brief Collects information about operating system up time and populates SystemInfo with it.
     * @param system_info - struct holds system monitoring information.
     */
    void collect_uptime(SystemInfo &);

    /** @brief Collects information about system load average and populates SystemInfo with it.
     * @param system_info - struct holds system monitoring information.
     */
    void collect_load(SystemInfo &);

    /** @brief Collects information about CPU usage SystemInfo with it.
     * @param system_info - struct holds system monitoring information.
     */
    void collect_cpu(SystemInfo &);

    /** @brief Collects information about memory usage SystemInfo with it.
     * @param system_info - struct holds system monitoring information.
     */
    void collect_memory(SystemInfo &);

    /** @brief Collects information about disks usage SystemInfo with it.
     * @param system_info - struct holds system monitoring information.
     */
    void collect_disks_usage(SystemInfo &);

    /** @brief Collects information about system processes SystemInfo with it.
     * @param system_info - struct holds system monitoring information.
     */
    void collect_processes_info(SystemInfo &);

    /** @brief Represents functionality for collecting common Linux specific data. */
    std::shared_ptr<CommonDataReaderLinux> m_common_data_reader;
    /** @brief Represents functionality to read and handle CPU load. */
    CpuReaderLinux m_cpu_reader;
    /** @brief Represents functionality to read and handle RAM. */
    MemoryReader m_mem_reader;
    /** @bried Represents functionality for generating descriptions of Linux processes. */
    ProcessBuilderLinux m_process_builder;
    /** @brief Pointer to instance of system info printer. */
    std::weak_ptr<IPrinter> m_printer_ptr;
};

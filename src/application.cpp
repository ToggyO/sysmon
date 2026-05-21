#include "application.hpp"

Application::Application()
    : m_system_info{},
      m_collector_ptr{SystemMonitorFactory{}.create()},
      m_printer_ptr{std::make_unique<FtxUiPrinter>(std::make_unique<SystemInfoProvider>(m_system_info))}
{
}

int Application::run(const volatile sig_atomic_t& stop)
{
    m_printer_ptr->print(m_system_info);

    // Data refreshing iteration duration
    size_t iteration_ms = 1500; // TODO:  // The delay between checks when to start a new data collection iteration
    auto quant_duration = std::chrono::milliseconds(iteration_ms / 20); // TODO: возможно, 20 в настройки вынести
    auto iteration_duration = std::chrono::milliseconds(iteration_ms);

    try
    {
        while (!stop)
        {
            auto start_time = std::chrono::high_resolution_clock::now();

            m_collector_ptr->collect(m_system_info);
            m_printer_ptr->print(m_system_info);

            await_for_new_iteration(stop, start_time, iteration_duration, quant_duration);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Sysmon exited with error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Application::await_for_new_iteration(const volatile sig_atomic_t& stop,
                                          const std::chrono::time_point<std::chrono::system_clock>& start_time,
                                          const std::chrono::milliseconds& iteration_duration,
                                          const std::chrono::milliseconds& quant_duration)
{
    // TODO: после получения SIGINT верхний цикл еще в течение {iteration_ms} не знает о получени сигнала + локализовать
    // на ENG В течение {iteration_ms} времени проверяем, пришел ли сигнал на остановку приложения в процессе итерации
    // сбора данных
    while (true)
    {
        if (stop)
        {
            break;
        }

        auto current_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);

        if (duration >= iteration_duration)
        {
            break;
        }

        std::this_thread::sleep_for(quant_duration);
    }
}

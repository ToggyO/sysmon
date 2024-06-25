#include <thread>

#include "system_monitor_factory.hpp"

#if defined(_WIN32) || defined(_WIN64)
#endif

#if defined(__APPLE__) && defined(__MACH__)
#endif

#if defined(linux) || defined(__linux__) || defined(__linux)
#include "linux/system_monitor/system_monitor.hpp"
#include "linux/pinter/ncurses_printer.hpp"

#endif

volatile sig_atomic_t stop;
void sig_handler(int signum) { stop = 1; }

// TODO: EPOLL LINUX для считывания системных файлов
// TODO: INOTIFY LINUX
// TODO: shared_ptr вместо сырых указателей
// TODO: ПРОВЕРИТЬ ПРАВИЛА ПЯТИ И КОЕ ГДЕ ЗАПРЕТИТЬ КОПИРОВАНИЕ

// TODO: НАТЫКАТЬ САНИТАЙЗЕРЫ
int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    SystemInfo system_info{};
    SystemMonitorFactory factory{};
    const auto system_monitor = factory.create();
    NCursesPrinter p(10); // TODO: реализовать фабрику для реализации принтера
    p.print(system_info);
    // TODO: добавить сбор initial value по CPU
    while (!stop)
    {
        system_monitor->collect(system_info);
        p.print(system_info); // TODO: поправить вывод RAM
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    // ВОПРОС: вероятно, очистка здесь лишняя, т.к. домен приложение в будет выгружен в любом случае после завершенния цикла
    // Очистка вызывает задержку в завершении программы
    factory.clear(system_monitor);
    return 0;
}

#include "sysmon_headers.h"

volatile sig_atomic_t stop;
void sig_stop_handler(int signum) { stop = 1; }

// TODO: ПРОВЕРИТЬ ПРАВИЛА ПЯТИ И КОЕ ГДЕ ЗАПРЕТИТЬ КОПИРОВАНИЕ
// TODO: НАТЫКАТЬ САНИТАЙЗЕРЫ
int main(int argc, char **argv)
{
    // TODO: ЭТО ЮНИКС СИГНАЛЫ ОЛОЛО. Для других ос не пойдет. Нужно инкапсулировать
    signal(SIGINT, sig_stop_handler);
    signal(SIGTERM, sig_stop_handler);
    signal(SIGTSTP, sig_stop_handler);
    signal(SIGABRT, sig_stop_handler);
//    signal(SIGWINCH, sig_resize_handler);

    SystemMonitorFactory factory{};
    return factory.create().run(stop);
}
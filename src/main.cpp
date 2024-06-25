#include "sysmon_headers.h"

volatile sig_atomic_t stop;
void sig_handler(int signum) { stop = 1; }

// TODO: ПРОВЕРИТЬ ПРАВИЛА ПЯТИ И КОЕ ГДЕ ЗАПРЕТИТЬ КОПИРОВАНИЕ
// TODO: НАТЫКАТЬ САНИТАЙЗЕРЫ
int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    SystemMonitorFactory factory{};
    return factory.create().run(stop);
}

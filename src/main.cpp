#include <cstddef>
#include <signal.h>

#include "application.hpp"

volatile static sig_atomic_t stop;
static void sig_stop_handler(int /*signum*/) { stop = 1; }

// TODO: файловые дексрипторы надо убрать! Либо закрывать сразу после чтения.
// Храниени в памяти приводит к too many open files в Linux
// TODO: добавить вывод htop's like Tasks
// TODO: cannot create std::vector larger than max_size() on ctrl+z
// TODO: clang-format + clang-tidy
// TODO: ПРОВЕРИТЬ ПРАВИЛА ПЯТИ И КОЕ ГДЕ ЗАПРЕТИТЬ КОПИРОВАНИЕ
// TODO: НАТЫКАТЬ САНИТАЙЗЕРЫ
int main(int argc, char **argv)
{
    // TODO: ЭТО ЮНИКС СИГНАЛЫ ОЛОЛО. Для других ос не пойдет. Нужно
    // инкапсулировать
    signal(SIGINT, sig_stop_handler);
    signal(SIGTERM, sig_stop_handler);
    signal(SIGTSTP, sig_stop_handler);
    signal(SIGABRT, sig_stop_handler);

    Application app;
    return app.run(stop);
}

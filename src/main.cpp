#include "sysmon_headers.h"

volatile static sig_atomic_t stop;
static void sig_stop_handler(int  /*signum*/) { stop = 1; }

// TODO: clang-format + clang-tidy
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

// TODO: remove
//#include <ftxui/component/screen_interactive.hpp>
//#include <ftxui/component/component.hpp>
//
//using namespace ftxui;
//
//int main(int argc, char **argv)
//{
//    auto renderer = Renderer([&] {
//        return hbox({
//            vbox({}) | border | size(WidthOrHeight::WIDTH, GREATER_THAN, 30),
//            filler(),
//            vbox({}) | border | size(WidthOrHeight::WIDTH, GREATER_THAN, 30),
//            filler(),
//            vbox({}) | border | size(WidthOrHeight::WIDTH, GREATER_THAN, 30),
//            filler(),
//            vbox({}) | border | size(WidthOrHeight::WIDTH, GREATER_THAN, 30),
//        });
//    });
//
//    auto screen = ScreenInteractive::TerminalOutput();
//    screen.Loop(renderer);
//    return 0;
//}
#pragma once

#include <atomic> // std::atomic_bool
#include "ftxui/component/component.hpp" // ftxui::Container
#include <ftxui/component/loop.hpp> // ftxui::Loop
#include <ftxui/component/screen_interactive.hpp> // ftxui::ScreenInteractive
#include <ftxui/dom/elements.hpp> // ftxui::hbox, ftxui::text, ftxui::border, ftxui::flex
#include <iomanip> // std::setprecision
#include <sstream> // std::stringstream

#include "sys_info/printer.interface.hpp" // IPrinter
#include "../../common/constants.hpp" // k_mem_units
#include "../../common/conversions.h" // GIGABYTES, KILOBYTES
#include "../../utils/utils.h" // elapsed_time


#include <csignal> // _SC_CLK_TCK TODO: check
#include <iostream>

using namespace ftxui;

// TODO: add descr
//class FtxUiPrinter : public IPrinter
//{
//private:
//    static void sig_resize_handler(int signum)
//    {
//        std::cout << "SIGWINCH " << signum;
//    }
//
//public:
//    explicit FtxUiPrinter() : m_screen{Screen::Create(
//        Dimension::Full(),
//        Dimension::Full()
//    )}
////    FtxUiPrinter()
//    {
//        signal(SIGWINCH, sig_resize_handler);
//    }
//
//    void print(const SystemInfo&) override;
//
//private:
//    Screen m_screen;
//};
// TODO: add descr
class FtxUiPrinter : public IPrinter
{
public:
    FtxUiPrinter() : m_screen{ScreenInteractive::TerminalOutput()}, m_initialized{false}
//        m_loop(&m_screen, /* default initialization */ Renderer([&]() {return hbox({});}))
    {
        std::cout << "FtxUiPrinter ctor called" << std::endl;
    }

    ~FtxUiPrinter()
    {
        std::cout << "FtxUiPrinter destructor called" << std::endl; // TODO: remove
        if (m_initialized.load())
        {
            m_loop_thread.join();
        }
        std::cout << "FtxUiPrinter destructor done" << std::endl; // TODO: remove
    }

    void print(const SystemInfo&) override;

private:
    ScreenInteractive m_screen;
    std::atomic_bool m_initialized;
    std::thread m_loop_thread; // TODO: check
//    std::unique_ptr<Loop> m_loop_ptr;
};
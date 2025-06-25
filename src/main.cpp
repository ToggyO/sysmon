#include "sysmon_headers.h"

volatile sig_atomic_t stop;
void sig_stop_handler(int signum) {
    std::cout << "Sig stop with code: " << signum << std::endl; // TODO: remove
    stop = 1;
}

// TODO: ПРОВЕРИТЬ ПРАВИЛА ПЯТИ И КОЕ ГДЕ ЗАПРЕТИТЬ КОПИРОВАНИЕ
// TODO: НАТЫКАТЬ САНИТАЙЗЕРЫ
int main(int argc, char **argv)
{
    // TODO: ЭТО ЮНИКС СИГНАЛЫ ОЛОЛО. Для других ос не пойдет. Нужно инкапсулировать
    std::signal(SIGINT, sig_stop_handler);
    std::signal(SIGTERM, sig_stop_handler);
//    signal(SIGWINCH, sig_resize_handler);

    SystemMonitorFactory factory{};
    return factory.create().run(stop);
}


// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
//#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
//#include <string>  // for string, basic_string
//#include <vector>  // for vector
//
//#include "ftxui/component/captured_mouse.hpp"  // for ftxui
//#include "ftxui/component/component.hpp"  // for Radiobox, Renderer, Tab, Toggle, Vertical
//#include "ftxui/component/component_base.hpp"      // for ComponentBase
//#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
//#include "ftxui/dom/elements.hpp"  // for Element, separator, operator|, vbox, border
//
//using namespace ftxui;
//
//int main() {
//    std::vector<std::string> tab_values{
//            "tab_1",
//            "tab_2",
//            "tab_3",
//    };
//    int tab_selected = 0;
//    auto tab_toggle = Toggle(&tab_values, &tab_selected);
//
//    std::vector<std::string> tab_1_entries{
//            "Forest",
//            "Water",
//            "I don't know",
//    };
//    int tab_1_selected = 0;
//
//    std::vector<std::string> tab_2_entries{
//            "Hello",
//            "Hi",
//            "Hay",
//    };
//    int tab_2_selected = 0;
//
//    std::vector<std::string> tab_3_entries{
//            "Table",
//            "Nothing",
//            "Is",
//            "Empty",
//    };
//    int tab_3_selected = 0;
//    auto tab_container = Container::Tab(
//            {
//                    Radiobox(&tab_1_entries, &tab_1_selected),
//                    Radiobox(&tab_2_entries, &tab_2_selected),
//                    Radiobox(&tab_3_entries, &tab_3_selected),
//            },
//            &tab_selected);
//
//    auto container = Container::Vertical({
//                                                 tab_toggle,
//                                                 tab_container,
//                                         });
//
//    auto renderer = Renderer(container, [&] {
//        return vbox({
//                            tab_toggle->Render(),
//                            separator(),
//                            tab_container->Render(),
//                    }) |
//               border;
//    });
//
////    auto screen = ScreenInteractive::TerminalOutput();
//    auto screen = ScreenInteractive::Fullscreen();
//    screen.Loop(renderer);
//}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

//#include <stddef.h>   // for size_t
//#include <algorithm>  // for max
//#include <memory>     // for allocator, shared_ptr
//#include <string>  // for char_traits, operator+, string, basic_string, to_string
//#include <utility>  // for move
//#include <vector>   // for vector
//
//#include "ftxui/component/captured_mouse.hpp"  // for ftxui
//#include "ftxui/component/component.hpp"       // for CatchEvent, Renderer
//#include "ftxui/component/event.hpp"           // for Event
//#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::Left, Mouse::Middle, Mouse::None, Mouse::Pressed, Mouse::Released, Mouse::Right, Mouse::WheelDown, Mouse::WheelUp
//#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
//#include "ftxui/dom/elements.hpp"  // for text, vbox, window, Element, Elements
//
//using namespace ftxui;
//
//std::string Code(Event event) {
//    std::string codes;
//    for (auto& it : event.input()) {
//        codes += " " + std::to_string((unsigned int)it);
//    }
//    return codes;
//}
//
//int main() {
//    auto screen = ScreenInteractive::TerminalOutput();
//
//    std::vector<Event> keys;
//
//    auto left_column = Renderer([&] {
//        Elements children = {
//                text("Codes"),
//                separator(),
//        };
//        for (size_t i = std::max(0, (int)keys.size() - 20); i < keys.size(); ++i) {
//            children.push_back(text(Code(keys[i])));
//        }
//        return vbox(children);
//    });
//
//    auto right_column = Renderer([&] {
//        Elements children = {
//                text("Event"),
//                separator(),
//        };
//        for (size_t i = std::max(0, (int)keys.size() - 20); i < keys.size(); ++i) {
//            children.push_back(text(keys[i].DebugString()));
//        }
//        return vbox(children);
//    });
//
//    int split_size = 40;
//    auto component = ResizableSplitLeft(left_column, right_column, &split_size);
//    component |= border;
//
//    component |= CatchEvent([&](Event event) {
//        keys.push_back(event);
//        return false;
//    });
//
//    screen.Loop(component);
//}
//#include <memory>
//#include <string>
//
//#include "ftxui/component/captured_mouse.hpp"
//#include "ftxui/component/component.hpp"
//#include "ftxui/component/component_base.hpp"
//#include "ftxui/component/screen_interactive.hpp"
//#include "ftxui/dom/elements.hpp"
//#include "ftxui/dom/table.hpp"
//
//using namespace ftxui;
//
//int main(int argc, const char* argv[]) {
//
//    std::string value_1;
//    std::string value_2;
//    std::string value_3;
//    std::string value_4;
//
//    auto input_1 = Input(value_1, "input_1");
//    auto input_2 = Input(value_2, "input_2");
//    auto input_3 = Input(value_3, "input_3");
//    auto input_4 = Input(value_4, "input_3");
//
//    // Layout combine the components above, so that user can navigate them using
//    // arrow keys in both directions. Unfortunately, ftxui doesn't provide a
//    // Container::Array component, so this is a best effort here:
//    auto layout = Container::Vertical({
//                                              Container::Horizontal({input_1, input_2}),
//                                              Container::Horizontal({input_3, input_4}),
//                                      });
//
//    // Renderer override the `ComponentBase::Render` function of `layout`, to
//    // display them using a Table.
//    auto renderer = Renderer(layout, [&] {
//        auto table = Table({
//                                   {text("")      , text("Columns 1") , text("Column2")}   ,
//                                   {text("row_1") , input_1->Render() , input_2->Render()} ,
//                                   {text("row_2") , input_3->Render() , input_4->Render()} ,
//                           });
//
//        table.SelectAll().Border(DOUBLE);
//        table.SelectAll().Separator(LIGHT);
//
//        return table.Render();
//    });
//
//    // Execute the main loop:
//    auto screen = ScreenInteractive::FitComponent();
//    screen.Loop(renderer);
//    return 0;
//}
#include "sysmon_headers.h"

volatile static sig_atomic_t stop;
static void sig_stop_handler(int /*signum*/) { stop = 1; }

// TODO: файловые дексрипторы надо убрать! Либо закрывать сразу после чтения. Храниени в памяти приводит к too many open files в Linux
// TODO: добавить вывод htop's like Tasks
// TODO: cannot create std::vector larger than max_size() on ctrl+z
// TODO: clang-format + clang-tidy
// TODO: ПРОВЕРИТЬ ПРАВИЛА ПЯТИ И КОЕ ГДЕ ЗАПРЕТИТЬ КОПИРОВАНИЕ
// TODO: НАТЫКАТЬ САНИТАЙЗЕРЫ

#include <memory>  // for shared_ptr, allocator, __shared_ptr_access

#include "ftxui/component/component.hpp"  // for Renderer, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, operator|, text, center, border

int main(int argc, char **argv)
{
    // TODO: ЭТО ЮНИКС СИГНАЛЫ ОЛОЛО. Для других ос не пойдет. Нужно инкапсулировать
    signal(SIGINT, sig_stop_handler);
    signal(SIGTERM, sig_stop_handler);
    signal(SIGTSTP, sig_stop_handler);
    signal(SIGABRT, sig_stop_handler);

    SystemMonitorFactory factory{};
    return factory.create().run(stop);



    // using namespace ftxui;

    // auto screen = ScreenInteractive::Fullscreen();

    // // State:
    // int left_size = 20;
    // int right_size = 20;
    // int top_size = 10;
    // int bottom_size = 10;

    

    // // Renderers:
    // auto RendererInfo = [](const std::string& name, int* size, Box& dimensions) {
    //     return Renderer([name, size, &dimensions] {
    //         std::stringstream ss;
    //         ss
    //             << "x_min=" << dimensions.x_min << ", "
    //             << "x_max=" << dimensions.x_max << ", "
    //             << "y_mix=" << dimensions.y_min << ", "
    //             << "y_max=" << dimensions.y_max << ", ";
    //         return text(name + ": " + std::to_string(*size) + ", " + ss.str()) | center | reflect(dimensions);
    //     });
    // };

    // Box midlle_box;
    // Box left_box;
    // Box right_box;
    // Box top_box;
    // Box bottom_box;

    // auto middle = Renderer([&midlle_box]
    // {
    //     std::stringstream ss;
    //     ss
    //         << "x_min=" << midlle_box.x_min << ", "
    //         << "x_max=" << midlle_box.x_max << ", "
    //         << "y_mix=" << midlle_box.y_min << ", "
    //         << "y_max=" << midlle_box.y_max << ", ";
    //     return text("Middle, " + ss.str()) | center | reflect(midlle_box);
    // });
    // auto left = RendererInfo("Left", &left_size, left_box);
    // auto right = RendererInfo("Right", &right_size, right_box);
    // auto top = RendererInfo("Top", &top_size, top_box);
    // auto bottom = RendererInfo("Bottom", &bottom_size, bottom_box);

    // auto container = middle;
    // container = ResizableSplitLeft(left, container, &left_size);
    // container = ResizableSplitRight(right, container, &right_size);
    // container = ResizableSplitTop(top, container, &top_size);
    // container = ResizableSplitBottom(bottom, container, &bottom_size);

    // auto renderer = Renderer(container, [&] { return container->Render() | border; });

    // screen.Loop(renderer);

    // return 0;
}




// #include <stdio.h>  // for getchar
// #include <ftxui/dom/elements.hpp>  // for Elements, gridbox, Fit, operator|, text, border, Element
// #include <ftxui/screen/screen.hpp>  // for Screen
// #include <memory>                   // for allocator, shared_ptr

// #include "ftxui/dom/node.hpp"      // for Render
// #include "ftxui/screen/color.hpp"  // for ftxui

// #include "./printer/ftxui_helpers.hpp" // FtxUiHelpers

// int main() {
//   using namespace ftxui;
//   // auto cell = [](const char* t) { return text(t) | border; };
//     auto cell = [](const char* t) { return text(t); };
//   auto document =  //
//       gridbox({
//           {
//               cell("north-west") | size(WidthOrHeight::WIDTH, EQUAL, 30) | size(WidthOrHeight::HEIGHT, EQUAL, 2) | vcenter,
//               cell("north") | size(WidthOrHeight::WIDTH, EQUAL, 100),
//               cell("north-east"),
//           },
//           {
//               cell("center-west") | size(WidthOrHeight::HEIGHT, EQUAL, 2) | vcenter,
//             cell("center-north-west"),
//               // gridbox({
//               //     {
//               //         cell("center-north-west"),
//               //         cell("center-north-east"),
//               //     },
//               //     {
//               //         cell("center-south-west"),
//               //         cell("center-south-east"),
//               //     },
//               // }),
//               cell("center-eastsddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddk"),
//           },
//           {
//               cell("south-west") | size(WidthOrHeight::HEIGHT, EQUAL, 2) | vcenter,
//               cell("south"),
//               cell("south-east"),
//           },
//       });
//   auto screen = Screen::Create(Dimension::Fit(document));
//   Render(screen, document);
//   screen.Print();
//   getchar();

//   return 0;
// }






// #include <ftxui/dom/elements.hpp>
// #include <ftxui/component/screen_interactive.hpp>
// #include <ftxui/component/component.hpp>
// #include <chrono>
// #include <thread>
// #include <vector>
// #include <random>

// using namespace ftxui;

// class DynamicTable {
// private:
//     std::vector<std::vector<std::string>> data;
//     std::vector<std::string> headers = {"ID", "Имя", "Статус", "Значение", "Время"};
//     std::random_device rd;
//     std::mt19937 gen;
//     std::uniform_int_distribution<> value_dist;
//     std::uniform_real_distribution<> time_dist;

// public:
//     DynamicTable() : gen(rd()), value_dist(0, 100), time_dist(0.0, 10.0) {
//         // Инициализация начальных данных
//         updateData();
//     }

//     void updateData() {
//         data.clear();
//         auto now = std::chrono::system_clock::now();

//         for (int i = 0; i < 10; ++i) {
//             std::vector<std::string> row;
//             row.push_back(std::to_string(i + 1)); // ID
//             row.push_back("Объект " + std::to_string(i + 1)); // Имя
//             row.push_back(i % 3 == 0 ? "Активен" : "Ожидание"); // Статус
//             row.push_back(std::to_string(value_dist(gen))); // Значение
//             row.push_back(std::to_string(time_dist(gen)) + "с"); // Время
//             data.push_back(row);
//         }
//     }

//     Component createTable() {
//         return Renderer([&] {
//             // Создаем элементы таблицы
//             Elements rows;

//             // Заголовок
//             Elements header_row;
//             for (const auto& header : headers) {
//                 header_row.push_back(text(header) | bold | center);
//             }
//             rows.push_back(hbox(std::move(header_row)));

//             // Разделитель
//             rows.push_back(text("") | border);

//             // Данные
//             for (const auto& row_data : data) {
//                 Elements row_elements;
//                 for (size_t i = 0; i < row_data.size(); ++i) {
//                     auto cell = text(row_data[i]);

//                     // Условное форматирование
//                     if (i == 2) { // Столбец статуса
//                         if (row_data[i] == "Активен") {
//                             cell = cell | color(Color::Green);
//                         } else {
//                             cell = cell | color(Color::Yellow);
//                         }
//                     } else if (i == 3) { // Столбец значения
//                         int value = std::stoi(row_data[i]);
//                         if (value > 80) {
//                             cell = cell | color(Color::Red) | bold;
//                         } else if (value > 50) {
//                             cell = cell | color(Color::Yellow);
//                         }
//                     }

//                     row_elements.push_back(cell | flex | center | size());
//                 }
//                 rows.push_back(hbox(std::move(row_elements)));
//             }

//             return vbox(std::move(rows)) | border;
//         });
//     }
// };

// int main() {
//     auto screen = ScreenInteractive::Fullscreen();
//     DynamicTable table;

//     // Компонент с автоматическим обновлением
//     auto component = Container::Vertical({});

//     auto table_component = table.createTable();

//     // Добавляем кнопку для ручного обновления
//     auto update_button = Button("Обновить данные", [&] {
//         table.updateData();
//     });

//     auto layout = Container::Vertical({
//         table_component,
//         update_button
//     });

//     // Автоматическое обновление каждые 2 секунды
//     std::atomic<bool> refresh_ui = true;
//     std::thread refresh_thread([&] {
//         while (refresh_ui) {
//             using namespace std::chrono_literals;
//             std::this_thread::sleep_for(2000ms);
//             table.updateData();
//             screen.PostEvent(Event::Custom); // Принудительное обновление экрана
//         }
//     });

//     screen.Loop(layout);

//     refresh_ui = false;
//     refresh_thread.join();

//     return 0;
// }




// #include <ftxui/dom/elements.hpp>  // for color, Fit, LIGHT, align_right, bold, DOUBLE
// #include <ftxui/dom/table.hpp>      // for Table, TableSelection
// #include <ftxui/screen/screen.hpp>  // for Screen
// #include <iostream>                 // for endl, cout, ostream
// #include <string>                   // for basic_string, allocator, string
// #include <vector>                   // for vector

// #include "ftxui/dom/node.hpp"  // for Render
// #include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Cyan, Color::White, ftxui

// int main() {
//   using namespace ftxui;

//   auto table = Table({
//       {"Version", "Marketing name", "Release date", "API level", "Runtime"},
//       {"2.3", "Gingerbread", "February 9 2011", "10", "Dalvik 1.4.0"},
//       {"4.0", "Ice Cream Sandwich", "October 19 2011", "15", "Dalvik"},
//       {"4.1", "Jelly Bean", "July 9 2012", "16", "Dalvik"},
//       {"4.2", "Jelly Bean", "November 13 2012", "17", "Dalvik"},
//       {"4.3", "Jelly Bean", "July 24 2013", "18", "Dalvik"},
//       {"4.4", "KitKat", "October 31 2013", "19", "Dalvik and ART"},
//       {"5.0", "Lollipop", "November 3 2014", "21", "ART"},
//       {"5.1", "Lollipop", "March 9 2015", "22", "ART"},
//       {"6.0", "Marshmallow", "October 5 2015", "23", "ART"},
//       {"7.0", "Nougat", "August 22 2016", "24", "ART"},
//       {"7.1", "Nougat", "October 4 2016", "25", "ART"},
//       {"8.0", "Oreo", "August 21 2017", "26", "ART"},
//       {"8.1", "Oreo", "December 5 2017", "27", "ART"},
//       {"9", "Pie", "August 6 2018", "28", "ART"},
//       {"10", "10", "September 3 2019", "29", "ART"},
//       {"11", "11", "September 8 2020", "30", "ART"},
//   });

//   table.SelectAll().Border(LIGHT);

//   // Add border around the first column.
//   table.SelectColumn(0).Border(LIGHT);
//   table.SelectColumn(0).DecorateCells(size(WidthOrHeight::WIDTH, EQUAL, 100));

//   // Make first row bold with a double border.
//   table.SelectRow(0).Decorate(bold);
//   table.SelectRow(0).SeparatorVertical(LIGHT);
//   table.SelectRow(0).Border(DOUBLE);

//   // Align right the "Release date" column.
//   table.SelectColumn(2).DecorateCells(align_right);

//   // Select row from the second to the last.
//   auto content = table.SelectRows(1, -1);
//   // Alternate in between 3 colors.
//   content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
//   content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
//   content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

//   auto document = table.Render();
//   auto screen =
//       Screen::Create(Dimension::Fit(document, /*extend_beyond_screen=*/true));
//   Render(screen, document);
//   screen.Print();
//   std::cout << std::endl;

//   return 0;
// }










// #include <ftxui/component/component.hpp>
// #include <ftxui/component/component_base.hpp>
// #include <ftxui/component/screen_interactive.hpp>
// #include <ftxui/dom/table.hpp>
// #include <algorithm>
// #include <vector>
// #include <random>

// using namespace ftxui;

// class SortableTable {
// private:
//     std::vector<std::vector<std::string>> data;
//     std::vector<std::string> headers = {"ID", "Имя", "Баланс", "Статус"};
//     int sort_column = 0;
//     bool ascending = true;

// public:
//     SortableTable() {
//         generateData();
//     }

//     void generateData() {
//         data.clear();
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_real_distribution<> balance_dist(0.0, 1000.0);

//         for (int i = 0; i < 8; ++i) {
//             std::vector<std::string> row;
//             row.push_back(std::to_string(1000 + i));
//             row.push_back("Пользователь " + std::to_string(i + 1));
//             row.push_back(std::to_string(balance_dist(gen)));
//             row.push_back(i % 2 == 0 ? "Online" : "Offline");
//             data.push_back(row);
//         }
//         sortData();
//     }

//     void sortData() {
//         std::sort(data.begin(), data.end(), [this](const auto& a, const auto& b) {
//             if (sort_column == 0 || sort_column == 2) { // Числовые колонки
//                 double val_a = std::stod(a[sort_column]);
//                 double val_b = std::stod(b[sort_column]);
//                 return ascending ? val_a < val_b : val_a > val_b;
//             } else { // Текстовые колонки
//                 return ascending ? a[sort_column] < b[sort_column]
//                                  : a[sort_column] > b[sort_column];
//             }
//         });
//     }

//     void setSortColumn(int col) {
//         if (sort_column == col) {
//             ascending = !ascending;
//         } else {
//             sort_column = col;
//             ascending = true;
//         }
//         sortData();
//     }

//     Component createTable() {
//         auto table_component = Renderer([this] {
//             Elements rows;

//             // Кликабельные заголовки для сортировки
//             Elements header_row;
//             for (size_t i = 0; i < headers.size(); ++i) {
//                 auto header_text = headers[i];
//                 if (static_cast<int>(i) == sort_column) {
//                     header_text += ascending ? " ▲" : " ▼";
//                 }

//                 auto header = text(header_text) | bold | center | flex;
//                 header_row.push_back(header);
//             }
//             rows.push_back(hbox(std::move(header_row)));
//             rows.push_back(text("") | border);

//             // Данные таблицы
//             for (const auto& row : data) {
//                 Elements row_elements;
//                 for (size_t i = 0; i < row.size(); ++i) {
//                     auto cell = text(row[i]);

//                     // Форматирование по типам данных
//                     if (i == 2) { // Баланс
//                         double balance = std::stod(row[i]);
//                         if (balance > 800) {
//                             cell = cell | color(Color::Green) | bold;
//                         } else if (balance < 200) {
//                             cell = cell | color(Color::Red);
//                         }
//                     } else if (i == 3) { // Статус
//                         if (row[i] == "Online") {
//                             cell = cell | color(Color::Green);
//                         } else {
//                             cell = cell | color(Color::GrayDark);
//                         }
//                     }

//                     row_elements.push_back(cell | flex | center);
//                 }
//                 rows.push_back(hbox(std::move(row_elements)));
//             }

//             return vbox(std::move(rows)) | border;
//         });

//         // Делаем заголовки кликабельными
//         auto clickable_table = CatchEvent(table_component, [this](Event event) {
//             if (event.is_mouse() && event.mouse().button == Mouse::Left &&
//                 event.mouse().motion == Mouse::Pressed) {

//                 // Простая логика определения колонки (можно улучшить)
//                 // В реальном приложении нужно рассчитывать позиции колонок
//                 auto mouse_x = event.mouse().x;
//                 if (mouse_x < 10) setSortColumn(0);
//                 else if (mouse_x < 20) setSortColumn(1);
//                 else if (mouse_x < 30) setSortColumn(2);
//                 else setSortColumn(3);

//                 return true;
//             }
//             return false;
//         });

//         return clickable_table;
//     }
// };

// int main() {
//     auto screen = ScreenInteractive::Fullscreen();
//     SortableTable table;

//     auto table_component = table.createTable();
//     auto refresh_button = Button("Обновить", [&] { table.generateData(); });

//     auto layout = Container::Vertical({
//         table_component,
//         refresh_button
//     });

//     auto renderer = Renderer(layout, [&] {
//         return vbox({
//             text("Динамическая таблица с сортировкой") | bold | center,
//             table_component->Render(),
//             refresh_button->Render() | center
//         }) | border;
//     });

//     screen.Loop(renderer);
//     return 0;
// }

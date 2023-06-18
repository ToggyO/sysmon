cmake_minimum_required(VERSION 3.5)

project(console)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_library(console STATIC
    ncurses_printer.h
    ncurses_printer.cpp
)

set_property(TARGET console PROPERTY CXX_STANDARD 17)
target_link_libraries(console ${CURSES_LIBRARIES})


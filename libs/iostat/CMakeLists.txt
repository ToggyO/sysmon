cmake_minimum_required(VERSION 3.5)

project(iostat)

add_library(iostat STATIC
    common.cpp
    common.h
    count.cpp
    count.h
    ioconf.cpp
    ioconf.h
    iostat.cpp
    iostat.h
    rd_stats.cpp
    rd_stats.h
    sysconfig.h
    systest.cpp
    systest.h
    version.h
)

# target_include_directories(
#     iostat
#     PUBLIC
#     ${CMAKE_CURRENT_BINARY_DIR}
# )

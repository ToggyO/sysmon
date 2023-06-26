cmake_minimum_required(VERSION 3.5)

project(sysmon)

add_subdirectory(src)
include_directories(include)

enable_testing()

add_subdirectory(tests)
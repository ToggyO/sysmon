cmake_minimum_required(VERSION 3.5)

project(sysmon)

add_subdirectory(src)
add_subdirectory(libs)
include_directories(include)

enable_testing()

add_subdirectory(tests)
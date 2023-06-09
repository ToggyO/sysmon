cmake_minimum_required(VERSION 3.5)

project(sysmon.src CXX)

# set common source files
set(COMMON_SOURCES
        "main.cpp"
        "sys_info/system_info.hpp"
        "common/system_monitor.hpp"
        "common/error.cpp"
        "utils/utils.cpp")

# detect system type
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(METER_SOURCES "common/load_average.cpp" "linux/cpu.cpp")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(METER_SOURCES)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(METER_SOURCES )
else()
    message(FATAL_ERROR "Unsupported OS - '${CMAKE_SYSTEM_NAME}'")
endif()

add_executable(sysmon ${COMMON_SOURCES} ${METER_SOURCES})
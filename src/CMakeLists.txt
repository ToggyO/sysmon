cmake_minimum_required(VERSION 3.5)

project(sysmon.src CXX)

# set common source files
set(COMMON_SOURCES
    "main.cpp"
    #"common/system_monitor.hpp"
    #"common/load_average.cpp"
    "common/error.cpp"
    "utils/utils.cpp"
)

# detect system type
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    file(GLOB METER_SOURCES "linux/*.cpp" "linux/*.hpp" "linux/**/*.cpp" "linux/**/*.hpp")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(METER_SOURCES)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(METER_SOURCES)
else()
    message(FATAL_ERROR "Unsupported OS - '${CMAKE_SYSTEM_NAME}'")
endif()

add_executable(sysmon ${COMMON_SOURCES} ${METER_SOURCES})

# detect system type to add OS specific libraries
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIRS})
    set_property(TARGET sysmon PROPERTY CXX_STANDARD 17)
    target_link_libraries(sysmon ${CURSES_LIBRARIES}) 

elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
else()
    message(FATAL_ERROR "Unsupported OS - '${CMAKE_SYSTEM_NAME}'")
endif()

target_include_directories(
	sysmon
	PRIVATE
	${CMAKE_SOURCE_DIR}/lib
    ../include
)

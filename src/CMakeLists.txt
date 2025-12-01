cmake_minimum_required(VERSION 3.5)

# set common source files
set(EXEC_SOURCES
    "main.cpp"
    "common/error.cpp"
    "utils/utils.cpp"
    "application.cpp"
    "application.hpp"
)

# TODO: запихать бы в COMMON_SOURCES
file(GLOB COMMON_SOURCES
    "printer/*.hpp"
    "printer/*.cpp"
    "provider/*.hpp"
    "provider/*.cpp"
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

add_executable(${INSTALL_TARGET} ${EXEC_SOURCES} ${COMMON_SOURCES} ${METER_SOURCES})

# detect system type to add OS specific libraries
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # Ncurses
#    find_package(Curses REQUIRED) # TODO: check or remove
#    include_directories(${CURSES_INCLUDE_DIRS})
    #

#    find_package(ftxui REQUIRED)
    # ftxui
    include(FetchContent)

    FetchContent_Declare(ftxui # TODO: попробовать с find_package
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
        GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
    )

    FetchContent_MakeAvailable(ftxui)
    #

    # TODO: проверить, какие компоненты frxui можно выпилить, чтобы ускорить сборку
    target_link_libraries(${INSTALL_TARGET}
#        PRIVATE ${CURSES_LIBRARIES}
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
    )

    set_property(TARGET ${INSTALL_TARGET} PROPERTY CXX_STANDARD 20)

elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
else()
    message(FATAL_ERROR "Unsupported OS - '${CMAKE_SYSTEM_NAME}'")
endif()

target_include_directories(
    ${INSTALL_TARGET}
	PRIVATE
	${CMAKE_SOURCE_DIR}/lib
    ../include
)

message(STATUS ${CMAKE_SOURCE_DIR})
message(STATUS ${INSTALL_TARGET})


cmake_minimum_required(VERSION 3.21)

# TODO: check
# Find clang-format executable
find_program(CLANG_FORMAT_EXECUTABLE clang-format)

if (CLANG_FORMAT_EXECUTABLE)
    # Define a custom target for formatting
    add_custom_target(clangformat
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -style=file -i ${SOURCE_FILES}
        COMMENT "Formatting source files with clang-format"
    )
else()
    message(WARNING "clang-format not found. Code formatting target will not be available.")
endif()

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(sysmon VERSION ${PROJECT_VERSION} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(INSTALL_TARGET ${PROJECT_NAME} CACHE INTERNAL "")

add_subdirectory(src)
include_directories(include)
add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT togushakov@gmail.com)
include(CPack)

target_include_directories(${INSTALL_TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/src")


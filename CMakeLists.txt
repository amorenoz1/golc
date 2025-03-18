cmake_minimum_required(VERSION 3.10)
project(GameOfLife C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Raylib
find_package(raylib REQUIRED)

# Source files
file(GLOB SOURCES main.c)

add_executable(GameOfLife ${SOURCES})

# Link Raylib
target_link_libraries(GameOfLife raylib)

# Include Raylib headers
include_directories(${RAYLIB_INCLUDE_DIRS})

# Enable debugging symbols
set(CMAKE_BUILD_TYPE Debug)

set_target_properties(GameOfLife PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# OS-specific settings
if (APPLE)
    target_link_libraries(GameOfLife "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
elseif(UNIX)
    target_link_libraries(GameOfLife m pthread dl rt X11)
endif()

cmake_minimum_required(VERSION 3.12)
project(spearstake)

# Compiler and Compiler Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall")

# Set the build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Source Files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Include and Library Paths
include_directories(/usr/include)
link_directories(/usr/lib)

# Executable
add_executable(spearstake ${SOURCES})

# Libraries
target_link_libraries(spearstake GL GLU glfw wayland-client wayland-cursor wayland-egl xkbcommon EGL GLESv2 pthread GLEW SOIL)

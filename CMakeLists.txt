cmake_minimum_required(VERSION 3.10)

# Project name and version
project(O-an-quan VERSION 1.0)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and header files
set(SOURCES
    src/main.cpp
)

# Exe
add_executable(O-an-quan ${SOURCES})


# Optional: copy asset folder into build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

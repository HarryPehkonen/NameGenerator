cmake_minimum_required(VERSION 3.15)
project(NameGenerator VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add include directory
include_directories(include)

# Create the executable
add_executable(namegen
    src/main.cpp
    src/NameGenerator.cpp
)

# Target include directories
target_include_directories(namegen PRIVATE include)

# Platform-specific settings
if(MSVC)
    # Windows-specific flags
    target_compile_options(namegen PRIVATE /W4)
else()
    # Linux/macOS flags
    target_compile_options(namegen PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Enable optimizations for release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(namegen PRIVATE /O2)
    else()
        target_compile_options(namegen PRIVATE -O3)
    endif()
endif()

# Installation
install(TARGETS namegen DESTINATION bin)

# Print configuration info
message(STATUS "NameGenerator Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")

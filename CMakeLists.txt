cmake_minimum_required(VERSION 3.15)
project(NameGenerator VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add include directory
include_directories(include)

# Try to find JSOM library installed on system
find_package(JSOM QUIET)

# If not found, fetch it from GitHub
if(NOT JSOM_FOUND)
    message(STATUS "JSOM not found on system, fetching from GitHub...")
    include(FetchContent)

    FetchContent_Declare(
        JSOM
        GIT_REPOSITORY https://github.com/HarryPehkonen/JSOM.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )

    # Fetch JSOM but don't build tests/benchmarks (we only need the library)
    FetchContent_GetProperties(JSOM)
    if(NOT jsom_POPULATED)
        FetchContent_Populate(JSOM)

        # Add only the library target, skip tests and other executables
        add_library(jsom_lib
            ${jsom_SOURCE_DIR}/src/json_document_pointer.cpp
            ${jsom_SOURCE_DIR}/src/json_document_formatting.cpp
        )
        target_include_directories(jsom_lib PUBLIC
            ${jsom_SOURCE_DIR}/include
        )
        target_compile_features(jsom_lib PRIVATE cxx_std_17)
    endif()

    message(STATUS "JSOM fetched successfully")
else()
    message(STATUS "Using system JSOM library")
endif()

# Create the executable
add_executable(namegen
    src/main.cpp
    src/NameGenerator.cpp
    src/ProfileData.cpp
)

# Target include directories
target_include_directories(namegen PRIVATE include)

# Link against JSOM library
target_link_libraries(namegen PRIVATE jsom_lib)

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

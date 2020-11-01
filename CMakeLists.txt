cmake_minimum_required(VERSION 3.15)
project(sniffer
        VERSION 1.0.0
        DESCRIPTION "Add two numbers"
        HOMEPAGE_URL "https://github.com/chernetskyi/cpp-template"
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Options
option(WARNINGS_AS_ERRORS   "Treat compiler warnings as errors."    ON)
option(ENABLE_CONAN         "Use Conan as a package manager."       OFF)
option(ENABLE_PVS_STUDIO    "Use PVS-Studio static code analyzer."  OFF) # Option for the local usage only. PVS-Studio isn't installed on GitHub machines.
option(ENABLE_SANITIZERS    "Use sanitizers to detect errors."      OFF) # Option for the test builds. Do not use it in the production builds.

include(cmake/extra/FindPCAP.cmake)

# Project source compilation
include_directories(inc)
add_library(utils STATIC
        src/utils/utils.cpp inc/util/utils.h)

add_library(core STATIC
        src/core/init.cpp src/core/socket/ethernet.cpp src/core/socket/ip.cpp inc/core/core.h src/core/socket/tcp.cpp)

add_library(IO STATIC
        src/IO/IO.cpp inc/IO/IO.h)

add_executable(sniffer src/main.cpp)
target_link_libraries(sniffer utils core IO ${PCAP_LIBRARY})

# Define ALL_TARGETS variable to use in some directives
set(ALL_TARGETS utils sniffer)

# Include default CMake configuration
include(cmake/Config.cmake)

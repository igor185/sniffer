cmake_minimum_required(VERSION 3.15)
project(sniffer
        VERSION 1.0.0
        DESCRIPTION "Add two numbers"
        HOMEPAGE_URL "https://github.com/chernetskyi/cpp-template"
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

find_package(Qt5 COMPONENTS Widgets REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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
        src/core/init.cpp
        src/core/socket/ethernet.cpp
        inc/core/ip.h
        src/core/socket/ip.cpp
        inc/core/core.h
        inc/core/tcp.h
        src/core/socket/tcp.cpp
        src/core/handlers.cpp
        src/core/socket/general.cpp inc/core/udp.h src/core/socket/udp.cpp)

add_library(IO STATIC
        src/IO/IO.cpp inc/IO/IO.h)

add_library(UI STATIC
        src/UI/InitPage/InitPage.cpp
        src/UI/InitPage/InitPage.h
        inc/UI/UI.h
        src/UI/init_gui.cpp
        src/UI/Table/Table.h
        src/UI/Table/table.cpp
        inc/UI/UI.h
        src/UI/Details/details.cpp
        src/UI/Details/details.h
        src/UI/Details/QHexView.cpp
        inc/UI/QHexView.h
        src/UI/Filters/ProxyModel.h
        src/UI/Filters/filters.h
        src/UI/Filters/filters.cpp
        src/UI/Filters/ProxyModel.cpp)


list(APPEND RESOURCES resources/resources.qrc)
qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_executable(sniffer ${RCC_SOURCES} src/main.cpp)

target_link_libraries(UI Qt5::Widgets core)
target_link_libraries(sniffer Qt5::Widgets UI utils core IO ${PCAP_LIBRARY})

# Define ALL_TARGETS variable to use in some directives
set(ALL_TARGETS utils sniffer)

# Include default CMake configuration
include(cmake/Config.cmake)

cmake_minimum_required(VERSION 3.15)
project(sniffer
        VERSION 1.0.0
        DESCRIPTION "Packet capturing"
        HOMEPAGE_URL "https://github.com/igor185/sniffer"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

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
include_directories(src)


set(utils_list
        src/utils/utils.cpp
        src/utils/utils.h
        src/utils/cli_parser.cpp
        src/utils/print_payload.cpp
        src/utils/files.cpp)
set(core_list
        src/core/core/init.cpp
        src/core/socket/ethernet.cpp
        src/core/socket/ip.cpp
        src/core/core.h
        src/core/socket/tcp.cpp
        src/core/core/handlers.cpp
        src/core/socket/general.cpp
        src/core/socket/udp.cpp
        src/core/socket/arp.cpp
        src/core/structs.h
        src/core/sockets.h)
set(ui_list
        src/UI/InitPage/InitPage.cpp
        src/UI/InitPage/InitPage.h
        src/UI/UI.h
        src/UI/init_gui.cpp
        src/UI/Table/Table.h
        src/UI/Table/table.cpp
        src/UI/Details/details.cpp
        src/UI/Details/details.h
        src/UI/HexView/QHexView.cpp
        src/UI/HexView/QHexView.h
        src/UI/Filters/ProxyModel.h
        src/UI/Filters/filters.h
        src/UI/Filters/filters.cpp
        src/UI/Filters/ProxyModel.cpp)

add_library(utils STATIC ${utils_list})
add_library(IO STATIC src/IO/IO.cpp src/IO/IO.h)
add_library(core STATIC ${core_list})
add_library(UI STATIC ${ui_list})
add_library(config STATIC src/config.h src/core/sniffer.cpp)


list(APPEND RESOURCES resources/resources.qrc)
qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_executable(sniffer ${RCC_SOURCES} src/main.cpp)

target_link_libraries(UI Qt5::Widgets core config)
target_link_libraries(sniffer Qt5::Widgets UI utils core IO ${PCAP_LIBRARY})

# Define ALL_TARGETS variable to use in some directives
set(ALL_TARGETS utils sniffer)

# Include default CMake configuration
include(cmake/Config.cmake)

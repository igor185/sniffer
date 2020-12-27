// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_IO_H
#define SNIFFER_IO_H

#include <string>

#include "core/core.h"

namespace IO {
    void print_help();

    void print(const std::string& line);

    void print_err(const std::string& line);

    void print_socket(std::vector<sockets::detail_view>&& view);
}
#endif //SNIFFER_IO_H

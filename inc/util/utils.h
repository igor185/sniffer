// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef ADDER_UTILS_H
#define ADDER_UTILS_H

#include "core/core.h"

namespace utils {
    core::config cli_parser(int argc, char** argv);
    std::string get_time(const timeval& ts);
    void print_payload(const u_char *payload, int len);

    bool is_integer(const std::string &s);
    int to_number(const std::string &s);
}

#endif //ADDER_UTILS_H

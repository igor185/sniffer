// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef ADDER_UTILS_H
#define ADDER_UTILS_H

#include <vector>

#include "core/core.h"
#include "config.h"

namespace utils {
    config cli_parser(int argc, char** argv);
    void print_payload(const u_char *payload, int len);

    bool is_integer(const std::string &s);
    long to_number(const std::string &s);
    std::vector<std::string> get_recent_files();
    void set_recent_file(const std::string& file);
}

#endif //ADDER_UTILS_H

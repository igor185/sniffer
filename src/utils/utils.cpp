// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <regex>
#include <string>

#include "utils.h"


bool utils::is_integer(const std::string &s) {
    return std::regex_match(s, std::regex("[0-9]+"));
}

int utils::to_number(const std::string &s) {
    char *end;
    if(is_integer(s)) {
        return std::strtol(s.c_str(), &end, 10);
    }
    return 0;
}


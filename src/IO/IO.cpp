// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <iostream>

#include "IO/IO.h"

void IO::print(const std::string &line) {
    std::cout << line << std::endl;
}

void IO::print_err(const std::string &line) {
    std::cerr << line << std::endl;
}

void IO::print_help() {
    print("Usage of sniffer: \n"
          "[-h|--help] - print this message \n"
          "[-d|--device name] - use some device to listen\n"
          "[-f|--file name] - redirect to file in specific format \n"
          "[-ff| --from-file name] - output file"
          "[-a|--amount number] - number of packages to handle (-1 - without limit)"
          "[-o|--out] - write to file and stdio (if --file present)"
          "[-p|--protocol name] - to filter using protocol name(doesn't work for reading from file TODO)");

    exit(EXIT_SUCCESS);
}
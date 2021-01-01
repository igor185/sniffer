// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <cstring>

#include "core/core.h"
#include "utils/utils.h"
#include "IO/IO.h"


config utils::cli_parser(int argc, char **argv) {
    config config = {};
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            IO::print_help();
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0) {
            if (argc - 1 == i)
                IO::print_help();
            config.device = argv[i++];
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            if (argc - 1 == i)
                IO::print_help();
            config.to_file_name = argv[++i];
            config.to_file = true;
        } else if (strcmp(argv[i], "-ff") == 0 || strcmp(argv[i], "--from-file") == 0) {
            if (argc - 1 == i)
                IO::print_help();
            config.from_file_name = argv[++i];
            config.from_file = true;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--amount") == 0) {
            if (argc - 1 == i && !is_integer(argv[i] + 1))
                IO::print_help();
            config.amount = to_number(argv[++i]);
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0) {
            config.both_out = true;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--protocol") == 0) {
            if (argc - 1 == i)
                IO::print_help();
            config.protoc_filter = argv[++i];
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--console") == 0) {
            config.console = true;
        }
    }
    return config;
}
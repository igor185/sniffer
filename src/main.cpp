// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "util/utils.h"
#include "core/core.h"

int main(int argc, char **argv) {

    auto config = utils::cli_parser(argc, argv);

    if (!config.from_file) {
        core::init_listening(config);
    } else {
        core::print_from_file(config);
    }

    return 0;
}

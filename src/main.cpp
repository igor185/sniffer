// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "utils/utils.h"
#include "core/core.h"
#include "UI/UI.h"

int main(int argc, char **argv) {
    auto config = utils::cli_parser(argc, argv);

    if(!config.console){
        return UI::init_gui(argc, argv, config);
    }

    config.device = "enx00e04c360457";

    core::init_listening(config);

    return 0;
}
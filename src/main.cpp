// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <csignal>
#include "utils/utils.h"
#include "core/core.h"
#include "UI/UI.h"

int main(int argc, char **argv) {
    auto config = utils::cli_parser(argc, argv);

    if(!config.console){
        return UI::init_gui(argc, argv, config);
    }

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = core::control_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, nullptr);

    config.device = "enx00e04c360457"; // TODO

    core::sniffer sniffer(config);

    sniffer.start_listen();

    return 0;
}
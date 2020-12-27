// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"
#include "IO/IO.h"

void core::sniffer::pause_listening() {
    if (is_listen) {
        pcap_breakloop(device);
        is_listen = false;
    } else {
        start_listen();
    }
}

void core::sniffer::start_listen() {
    is_listen = true;
    if (!configs.device.empty()) { // TODO
        init_listening_online();
    } else {
        init_listening_offline();
    }
}

void core::sniffer::init_listening_online() {
    struct bpf_program fp{};

    device = pcap_open_live(configs.device.c_str(), BUFSIZ, 1, -1, err_buf);
    if (device == nullptr) {
        IO::print_err(err_buf);
        return;
    }

    if (!configs.protoc_filter.empty()) {
        if (pcap_compile(device, &fp, configs.protoc_filter.c_str(), 0, 0) == -1) {
            IO::print_err("Couldn't parse filter: " + configs.protoc_filter);
            exit(EXIT_FAILURE);
        }

        if (pcap_setfilter(device, &fp) == -1) {
            IO::print_err("Couldn't install filter");
            exit(EXIT_FAILURE);
        }
    }

    if (!configs.console) {
        pcap_loop(device, configs.amount, core::ui_handler, nullptr);
    } else if (configs.to_file) {
        pcap_dumper_t *dumpfile = pcap_dump_open(device, configs.to_file_name.c_str());
        pcap_loop(device, configs.amount, core::file_handler, (unsigned char *) dumpfile);
    } else {
        pcap_loop(device, configs.amount, core::console_handler, nullptr);
    }
}

void core::sniffer::init_listening_offline() {
    pcap_t *dumpfile = pcap_open_offline(configs.from_file_name.c_str(), err_buf);

    IO::print_err(err_buf); // TODO
    if (dumpfile == nullptr) {
        IO::print(err_buf);
        exit(EXIT_FAILURE);
    }

    if (!configs.console) {
        pcap_loop(dumpfile, configs.amount, &core::ui_handler, nullptr);
    } else {
        pcap_loop(dumpfile, configs.amount, &core::console_handler, nullptr);
    }
}

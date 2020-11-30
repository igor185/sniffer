// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <pcap.h>
#include <cstdio>
#include <iostream>

#include "core/core.h"
#include "IO/IO.h"

pcap_t *dsc = nullptr;

void core::pause_capturing() {
    if(dsc != nullptr){
        pcap_breakloop(dsc);
    }
}

void init_listening_online(const core::config &config) {
    char err_buf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp{};

    dsc = pcap_open_live(config.device.c_str(), BUFSIZ, 1, -1, err_buf);
    if (dsc == nullptr) {
        IO::print(err_buf);
        return;
    }

    if (!config.protoc_filter.empty()) {
        if (pcap_compile(dsc, &fp, config.protoc_filter.c_str(), 0, 0) == -1) {
            IO::print_err("Couldn't parse filter: " + config.protoc_filter);
            exit(EXIT_FAILURE);
        }

        if (pcap_setfilter(dsc, &fp) == -1) {
            IO::print("Couldn't install filter");
            exit(EXIT_FAILURE);
        }
    }

    if (config.gui) {
        pcap_loop(dsc, config.amount, core::ui_handler, nullptr);
    } else if (config.to_file) {
        pcap_dumper_t *dumpfile = pcap_dump_open(dsc, config.to_file_name.c_str());
        pcap_loop(dsc, config.amount, core::file_handler, (unsigned char *) dumpfile);
    } else {
        pcap_loop(dsc, config.amount, core::console_handler, nullptr);
    }
}

void init_listening_offline(const core::config &config) {
    char err_buf[PCAP_ERRBUF_SIZE];
    pcap_t *dumpfile = pcap_open_offline(config.from_file_name.c_str(), err_buf);

    std::cout << err_buf << std::endl;
    IO::print("here");
    if (dumpfile == nullptr) {
        IO::print(err_buf);
        exit(EXIT_FAILURE);
    }

    if (config.gui) {
        pcap_loop(dumpfile, config.amount, &core::ui_handler, nullptr);
    } else {
        pcap_loop(dumpfile, config.amount, &core::console_handler, nullptr);
    }
}


void core::init_listening(const core::config &config) {

    if (!config.device.empty()) {
        init_listening_online(config);
    } else {
        init_listening_offline(config);
    }
}

std::vector<std::string> core::device_list() {
    std::vector<std::string> devices;

    pcap_if_t *alldevs;
    pcap_if_t *d;

    char err_buf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, err_buf) == -1) {
        fprintf(stderr, "Error in pcap_find all devs_ex: %s\n", err_buf);
        exit(EXIT_FAILURE);
    }

    for (d = alldevs; d != nullptr; d = d->next)
        devices.emplace_back(d->name);


    return devices;
}

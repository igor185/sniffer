// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <pcap.h>
#include <cstdio>
#include <iostream>

#include "core/core.h"
#include "IO/IO.h"


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

void sockets::base_socket::print() { // TODO
    IO::print_socket(to_view());
}
sockets::base_socket::base_socket(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet): packet(packet),
        pkt_hdr(pkt_hdr),
        cap_len(pkt_hdr->caplen),
        len(pkt_hdr->len){
}

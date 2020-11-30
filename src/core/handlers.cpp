// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"
#include "UI/UI.h"
#include "IO/IO.h"
#include <iostream>


void core::console_handler(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) {
    sockets::base_socket *sk = sockets::parse_packet(args, pkt_hdr, packet);

    if (sk != nullptr)
        sk->print();
}

void core::file_handler(u_char *dumpfile, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) {
    pcap_dump(dumpfile, pkt_hdr, packet);
}

void core::ui_handler(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) {
    UI::add_to_table(pkt_hdr, packet);
}

void core::write_to_file(core::config& config, std::vector<const struct pcap_pkthdr *> &pkt_hdr, std::vector<const u_char *> &packet) {
    char err_buf[PCAP_ERRBUF_SIZE];
    pcap_t *dsc = pcap_open_live(config.device.c_str(), BUFSIZ, 1, -1, err_buf);
    std::cout << err_buf << std::endl;
    pcap_dumper_t *dumpfile = pcap_dump_open(dsc, config.to_file_name.c_str());
    IO::print("open");
    for(size_t i = 0; i< pkt_hdr.size(); i++){
        core::file_handler((unsigned char *) dumpfile, pkt_hdr[i], packet[i]);
    }
}
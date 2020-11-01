// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"
#include "IO/IO.h"
#include "util/utils.h"

sockets::tcp::tcp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) :
        ip(args, pkt_hdr, packet), tcp_ptr((struct sniff_tcp *) (packet + SIZE_ETHERNET)) {
}

void sockets::tcp::print() {
    if (pkt_hdr != nullptr && tcp_ptr != nullptr) {
        IO::print(utils::get_time(pkt_hdr->ts) + " " +
                  get_type() + " " +
                          std::to_string(ntohs(tcp_ptr->th_sport)) + " -> " +
                          std::to_string(ntohs(tcp_ptr->th_dport)) + " " + std::to_string(pkt_hdr->len - SIZE_ETHERNET)); // TODO size

//        int size_tcp = TH_OFF(tcp)*4;
//        utils::print_payload((u_char *)(packet + SIZE_ETHERNET + ip_size), ntohs(tcp_ptr->) - ip_size);
    }
}

std::string sockets::tcp::get_type() {
    return "TCP";
}
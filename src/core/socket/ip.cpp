// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"
#include "util/utils.h"
#include "IO/IO.h"

sockets::ip::ip(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) :
        ethernet(args, pkt_hdr, packet), ip_ptr((struct sniff_ip *) (packet + SIZE_ETHERNET)) {
}

void sockets::ip::print() {
    if (pkt_hdr != nullptr && ip_ptr != nullptr) {
        IO::print(utils::get_time(pkt_hdr->ts) + " " +
                  get_type() + " " +
                  inet_ntoa(ip_ptr->ip_src) + " -> " +
                  inet_ntoa(ip_ptr->ip_dst) + " " + std::to_string(pkt_hdr->len - SIZE_ETHERNET));

        int ip_size = IP_HL(ip_ptr)*4;
        utils::print_payload((u_char *)(packet + SIZE_ETHERNET + ip_size), ntohs(ip_ptr->ip_len) - ip_size);
    }
}

std::string sockets::ip::get_type() {
    if (ip_ptr == nullptr) {
        return "?";
    }

    switch (ip_ptr->ip_p) {
        case IPPROTO_IP:
            return "IP";
        case IPPROTO_ICMP:
            return "ICMP";
        case IPPROTO_IGMP:
            return "IGMP";
        case IPPROTO_IPIP:
            return "IPIP";
        case IPPROTO_TCP:
            return "TCP";
        case IPPROTO_EGP:
            return "EGP";
        case IPPROTO_PUP:
            return "PUP";
        case IPPROTO_UDP:
            return "UPD";
        case IPPROTO_IDP:
            return "IDP";
        case IPPROTO_TP:
            return "TP";
        case IPPROTO_DCCP:
            return "DCCP";
        case IPPROTO_IPV6:
            return "IPV6";

        default:
            return "*";
    }
}

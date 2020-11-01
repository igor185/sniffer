// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"
#include "util/utils.h"
#include "IO/IO.h"


sockets::ethernet::ethernet(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet):
         base_socket(args, pkt_hdr, packet), e_ptr((struct ether_header *) packet){
}

void sockets::ethernet::print() {
    if(pkt_hdr != nullptr && e_ptr != nullptr) {
        IO::print(utils::get_time(pkt_hdr->ts) + " " +
               get_type() + " " +
               ether_ntoa((struct ether_addr *) e_ptr->ether_shost) + " -> " +
               ether_ntoa((struct ether_addr *) e_ptr->ether_dhost) + " " + std::to_string(pkt_hdr->len - SIZE_ETHERNET));
    }
}

std::string sockets::ethernet::get_type() {
    if(e_ptr == nullptr) {
        return "?";
    }

    uint16_t type = ntohs(e_ptr->ether_type);

    switch (type) {
        case ETHERTYPE_IP:
            return "IP";
        case ETHERTYPE_ARP:
            return "ARP";
        case ETHERTYPE_REVARP:
            return "Reverse ARP";
        case ETHERTYPE_AARP:
            return "AppleTalk ARP";
        case ETHERTYPE_PUP:
            return "Xerox PUP";
        case ETHERTYPE_SPRITE:
            return "Sprite";
        case ETHERTYPE_AT:
            return "AppleTalk";
        case ETHERTYPE_VLAN:
            return "IEEE 802.1Q VLAN";
        case ETHERTYPE_IPX:
            return "IPX";
        case ETHERTYPE_IPV6:
            return "IPv6";
        case ETHERTYPE_LOOPBACK:
            return "loopback";
        default:
            return "?";
    }
}
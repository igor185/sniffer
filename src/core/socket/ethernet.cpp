// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"


sockets::ethernet::ethernet(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) :
        base_socket(args, pkt_hdr, packet), e_ptr((struct ether_header *) packet) {
}

sockets::table_view sockets::ethernet::_to_row() {
    struct sockets::table_view view;
    view.source = ether_ntoa((struct ether_addr *) e_ptr->ether_shost);
    view.destination = ether_ntoa((struct ether_addr *) e_ptr->ether_dhost);
    view.protocol = _get_type();
    view.size = pkt_hdr->len;


    return view;
}

std::string sockets::ethernet::_get_type() {
    if (e_ptr == nullptr) {
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

std::vector<sockets::detail_view>  sockets::ethernet::_to_view() {
    sockets::detail_view view{};

    view.preview = "Ethernet";

    view.properties = {std::make_pair("Source", ether_ntoa((struct ether_addr *) e_ptr->ether_shost)),
                       std::make_pair("Destination", ether_ntoa((struct ether_addr *) e_ptr->ether_dhost)),
                       std::make_pair("Type", get_type())};

    return {view};
}

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <core/core.h>


sockets::arp::arp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) : ip(args, pkt_hdr, packet),
                                                                                           arp_ptr((struct arp_hdr *) (
                                                                                                   packet +
                                                                                                   SIZE_ETHERNET +
                                                                                                   sizeof(sniff_ip))) {
}

std::string sockets::arp::_get_type() {
    return "ARP";
}

std::vector<sockets::detail_view> sockets::arp::_to_view() {
    std::vector<sockets::detail_view> data = ip::_to_view();

    sockets::detail_view view;

    view.preview = "Address Resolution Protocol";

    view.properties = {};

    view.properties.emplace_back("Hardware address", std::to_string(arp_ptr->ar_hrd));
    view.properties.emplace_back("Protocol address", std::to_string(arp_ptr->ar_pro));
    view.properties.emplace_back("Hardware length", std::to_string(arp_ptr->ar_hln));
    view.properties.emplace_back("Protocol length", std::to_string(arp_ptr->ar_pln));
    view.properties.emplace_back("Opcode", std::to_string(arp_ptr->ar_op));

    data.push_back(view);

    return data;
}

sockets::table_view sockets::arp::_to_row() {
    struct sockets::table_view view = ip::_to_row();
    view.protocol = _get_type();

    std::stringstream ss;
    hex_dump(ss, sizeof (ether_header) + sizeof(sniff_ip) + sizeof(arp_hdr) + (const char *)packet, pkt_hdr->len);

    view.info = ss.str();


    return view;
}

std::string sockets::arp::source_layer_(int type) {
    switch (type - 1) {
        case Physic:
        case Network:
            return sockets::ip::source_layer_(type);
        case Transport:
            return std::to_string(arp_ptr->ar_hrd);
        default:
            return "";
    }
}

std::string sockets::arp::destination_layer_(int type) {
    switch (type - 1) {
        case Physic:
        case Network:
            return sockets::ip::destination_layer_(type);
        default:
            return "";
    }
}

std::string sockets::arp::protocol_layer_(int type) {
    switch (type - 1) {
        case Physic:
        case Network:
            return sockets::ip::source_layer_(type);
        case Transport:
            return "ARP";
        default:
            return "";
    }
}
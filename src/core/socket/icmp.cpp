// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <core/core.h>


sockets::icmp::icmp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) : ip(args, pkt_hdr, packet), icmp_hdr((struct icmphdr *) (packet + SIZE_ETHERNET + sizeof(sniff_ip))) {}

std::string sockets::icmp::_get_type() {
    return "ICMP";
}

std::vector<sockets::detail_view> sockets::icmp::_to_view() {
    std::vector<sockets::detail_view> data = ip::_to_view();

    sockets::detail_view view;

    view.preview = "Internet Control Message Protocol";

    view.properties = {};

    view.properties.emplace_back("Type", std::to_string(icmp_hdr->type));
    view.properties.emplace_back("Code", std::to_string(icmp_hdr->code));

    data.push_back(view);

    return data;
}

sockets::table_view sockets::icmp::_to_row() {
    return ip::_to_row();
}

std::string sockets::icmp::source_layer_(int type) {
    return sockets::ip::source_layer_(type);
}

std::string sockets::icmp::destination_layer_(int type) {
    return sockets::ip::destination_layer_(type);
}

std::string sockets::icmp::protocol_layer_(int type) {
    switch (type - 1) {
        case Physic:
        case Network:
            return sockets::ip::protocol_layer_(type);
        case Transport:
            return "ICMP";
        default:
            return "";
    }
}
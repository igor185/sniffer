// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <core/core.h>


sockets::udp::udp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) :
        ip(args, pkt_hdr, packet), udp_ptr((struct udp_header *) (packet + SIZE_ETHERNET + sizeof(sniff_ip))) {
}

std::string sockets::udp::_get_type() {
    return "UDP";
}

std::vector<sockets::detail_view> sockets::udp::_to_view() {
    std::vector<sockets::detail_view> data = ip::_to_view();

    sockets::detail_view view;

    view.preview = "User Datagram Protocol";

    view.properties = {};

    view.properties.emplace_back("Source", std::to_string(ntohs(udp_ptr->udph_srcport)));
    view.properties.emplace_back("Destination", std::to_string(ntohs(udp_ptr->udph_destport)));
    view.properties.emplace_back("Length", std::to_string(udp_ptr->udph_chksum));
    view.properties.emplace_back("Checksum", std::to_string(udp_ptr->udph_len));

    data.push_back(view);

    return data;
}

sockets::table_view sockets::udp::_to_row() {
    struct sockets::table_view view;
    view.source = std::to_string(ntohs(udp_ptr->udph_srcport));
    view.destination = std::to_string(ntohs(udp_ptr->udph_destport));
    view.protocol = _get_type();
    view.size = pkt_hdr->len;


    return view;
}

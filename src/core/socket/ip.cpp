// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"


sockets::ip::ip(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) :
        ethernet(args, pkt_hdr, packet), ip_ptr((struct sniff_ip *) (packet + SIZE_ETHERNET)) {
}

std::string sockets::ip::_get_type() {
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
            return "UDP";
        case IPPROTO_IDP:
            return "IDP";
        case IPPROTO_TP:
            return "TP";
        case IPPROTO_DCCP:
            return "DCCP";
        case IPPROTO_IPV6:
            return "IPV6";

        default:
            return "IP*";
    }
}

std::string get_flag(u_short ip_off) {
    if (ip_off & IP_RF)
        return "0x8000, reserved fragment flag";
    if (ip_off & IP_DF)
        return "0x4000, don't fragment flag";
    if (ip_off & IP_MF)
        return "0x2000, more fragments flag";
    if (ip_off & IP_OFFMASK)
        return "0x1fff, mask for fragmenting bits";
    return "Unknown";
}

std::vector<sockets::detail_view> sockets::ip::_to_view() {
    std::vector<sockets::detail_view> data = ethernet::_to_view();

    sockets::detail_view view;

    view.preview = "Internet Protocol Version 4";

    view.properties = {};

    view.properties.emplace_back("Source", inet_ntoa(ip_ptr->ip_src));
    view.properties.emplace_back("Destination", inet_ntoa(ip_ptr->ip_src));
    view.properties.emplace_back("Header size", std::to_string(ip_ptr->ip_vhl >> 2));
    view.properties.emplace_back("Total length", std::to_string(ip_ptr->ip_len));
    view.properties.emplace_back("Flag", get_flag(ip_ptr->ip_off));
    view.properties.emplace_back("Time to life", std::to_string(ip_ptr->ip_ttl));
    view.properties.emplace_back("Checksum", std::to_string(ip_ptr->ip_sum));
    view.properties.emplace_back("Type", get_type());

    data.push_back(view);

    return data;
}

sockets::table_view sockets::ip::_to_row() {
    struct sockets::table_view view;
    view.source = inet_ntoa(ip_ptr->ip_src);
    view.destination = inet_ntoa(ip_ptr->ip_src);
    view.protocol = _get_type();
    view.size = pkt_hdr->len;


    return view;
}

std::string sockets::ip::source_layer_(int type) {
    switch (type - 1) {
        case Physic:
            return ethernet::source_layer_(type);
        case Network:
            return inet_ntoa(ip_ptr->ip_src);
        default:
            return "";
    }
}

std::string sockets::ip::destination_layer_(int type) {
    switch (type - 1) {
        case Physic:
            return ethernet::destination_layer_(type);
        case Network:
            return inet_ntoa(ip_ptr->ip_src);
        default:
            return "";
    }
}

std::string sockets::ip::protocol_layer_(int type) {
    switch (type - 1) {
        case Physic:
            return "ethernet";
        case Network:
            return "IPv4";
        default:
            return "";
    }
}

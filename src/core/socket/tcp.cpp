// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"
#include "IO/IO.h"

sockets::tcp::tcp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) :
        ip(args, pkt_hdr, packet), tcp_ptr((struct sniff_tcp *) (packet + SIZE_ETHERNET + sizeof(sniff_ip))) {
}

std::string get_flag(u_char  th_flags){
    if(th_flags & TH_FIN)
        return "0x01, FIN";
    if(th_flags & TH_SYN)
        return "0x02, SYN";
    if(th_flags & TH_RST)
        return "0x04, RST";
    if(th_flags & TH_PUSH)
        return "0x08, PUSH";
    if(th_flags & TH_ACK)
        return "0x10, ACK";
    if(th_flags & TH_URG)
        return "0x20, URG";
    if(th_flags & TH_ECE)
        return "0x40, ECE";
    if(th_flags & TH_CWR)
        return "0x80, CWR";
    return "Unknown";
}

std::string sockets::tcp::_get_type() {
    return "TCP";
}

std::vector<sockets::detail_view> sockets::tcp::_to_view() {
    std::vector<sockets::detail_view> data = ip::_to_view();

    sockets::detail_view view;

    view.preview = "Transmission Control Protocol";

    view.properties = {};

    view.properties.emplace_back("Source", std::to_string(ntohs(tcp_ptr->th_sport)));
    view.properties.emplace_back("Destination", std::to_string(ntohs(tcp_ptr->th_dport)));
    view.properties.emplace_back("Sequence Number", std::to_string(tcp_ptr->th_seq));
    view.properties.emplace_back("Acknowledgment Number", std::to_string(tcp_ptr->th_ack));
    view.properties.emplace_back("Flag", get_flag(tcp_ptr->th_flags));
    view.properties.emplace_back("Checksum", std::to_string(tcp_ptr->th_sum));

    data.push_back(view);

    return data;
}

sockets::table_view sockets::tcp::_to_row() {
    struct sockets::table_view view;
    view.source = std::to_string(ntohs(tcp_ptr->th_sport));
    view.destination = std::to_string(ntohs(tcp_ptr->th_dport));
    view.protocol = _get_type();
    view.size = pkt_hdr->len;


    return view;
}

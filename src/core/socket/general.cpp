// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "core/core.h"
#include "IO/IO.h"

sockets::base_socket* parse_transport(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet,
                                      const std::string &type) {
    sockets::base_socket *sock = nullptr;

    if (type == "TCP") {
        return new sockets::tcp(args, pkt_hdr, packet);
    }else if(type == "UDP"){
        return new sockets::udp(args, pkt_hdr, packet);
    } else if(type == "ICMP"){
        return new sockets::icmp(args, pkt_hdr, packet);
    }
    return sock;
}

sockets::base_socket* parse_network(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet, const std::string &type) {
    sockets::base_socket *sock = nullptr;

    if (type == "IP") {
        sock = new sockets::ip(args, pkt_hdr, packet);
    } else if (type == "ARP") {
        sock = new sockets::arp(args, pkt_hdr, packet);
    }

    if (sock != nullptr) {
        auto *sock_transport = parse_transport(args, pkt_hdr, packet, sock->get_type());
        if (sock_transport != nullptr)
            return sock_transport;
    }

    return sock;
}

sockets::base_socket* sockets::parse_packet(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) {
    if (pkt_hdr->caplen < ETHER_HDRLEN) {
        IO::print_err("Packet length less than ethernet header length");
        return nullptr;
    }
    auto *eth_sock = new ethernet(args, pkt_hdr, packet);

    auto *sock = parse_network(args, pkt_hdr, packet, eth_sock->get_type());

    if (sock != nullptr)
        return sock;

    return eth_sock;
}
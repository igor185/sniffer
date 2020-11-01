// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <pcap.h>
#include <cstdio>
#include <net/ethernet.h>

#include "core/core.h"
#include "IO/IO.h"

void another_callback(u_char *args, const struct pcap_pkthdr *pkt_hdr,
                      const u_char *packet) {
    sockets::base_socket *sk = sockets::parse_packet(args, pkt_hdr, packet);

    if (sk != nullptr && sk->get_type() != "?") {
            sk->print();
    }
}

void to_file(u_char *dumpfile, const struct pcap_pkthdr *pkt_hdr,
                      const u_char *packet) {
    pcap_dump(dumpfile, pkt_hdr, packet);
}

void core::init_listening(const core::config &config) {
    char err_buf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp{};


    pcap_t *dsc = pcap_open_live(config.device.c_str(), BUFSIZ, 1, -1, err_buf);
    if (dsc == nullptr) {
        IO::print(err_buf);
        return;
    }

    if(!config.protoc_filter.empty()){
        if (pcap_compile(dsc, &fp, config.protoc_filter.c_str(), 0, 0) == -1) {
            IO::print_err("Couldn't parse filter: " + config.protoc_filter);
            exit(EXIT_FAILURE);
        }

        if (pcap_setfilter(dsc, &fp) == -1) {
            IO::print("Couldn't install filter");
            exit(EXIT_FAILURE);
        }
    }

    if(config.to_file) {
        pcap_dumper_t *dumpfile = pcap_dump_open(dsc, config.file_name.c_str());

        pcap_loop(dsc, config.amount, to_file,  (unsigned char *)dumpfile);

        if(!config.both_out) { // TODO here is delay between to_file and another_callback
            return;
        }
    }

    pcap_loop(dsc, config.amount, another_callback, nullptr);
}


void core::print_from_file(const config &config) {
    char err_buf[PCAP_ERRBUF_SIZE];
    pcap_t *dumpfile = pcap_open_offline(config.from_file_name.c_str(), err_buf);


    if(dumpfile == nullptr){
        IO::print(err_buf);
        exit(EXIT_FAILURE);
    }

    if (pcap_loop(dumpfile, config.amount, &another_callback, nullptr) < 0) {
        IO::print_err("Error get sockets");
        exit(EXIT_FAILURE);
    }
}

sockets::base_socket *
sockets::parse_packet(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet) {
    if (pkt_hdr->caplen < ETHER_HDRLEN) {
        fprintf(stdout, "Packet length less than ethernet header length\n");
        return nullptr;
    }
    auto *eth_sock = new ethernet(args, pkt_hdr, packet);

    auto *sock = sockets::parse_network(args, pkt_hdr, packet, eth_sock->get_type());

    if (sock != nullptr)
        return sock;
    return eth_sock;
}

sockets::base_socket * sockets::parse_transport(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet,
                                                const std::string &type) {
    sockets::base_socket *sock = nullptr;

    if(type == "TCP"){
        return new tcp(args, pkt_hdr, packet);
    }
    return sock;
}

sockets::base_socket *
sockets::parse_network(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet, const std::string &type) {
    sockets::base_socket *sock = nullptr;

    if (type == "IP") {
        sock = new ip(args, pkt_hdr, packet);
    } else if (type == "IPv6") {
        // todo another types
    }

    if (sock != nullptr) {
        auto* sock_transport = sockets::parse_transport(args, pkt_hdr, packet, sock->get_type());
        if (sock_transport != nullptr)
            return sock_transport;
    }

    return sock;
}

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_CORE_H
#define SNIFFER_CORE_H

#include <pcap.h>
#include <vector>
#include <string>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <utility>
#include "core/structs.h"

namespace core {
    struct config {
        bool to_file = false;
        bool from_file = false;
        bool both_out = false; // TODO
        bool console = false;
        std::string device;
        std::string to_file_name = "log.pcap";
        std::string from_file_name = "log.pcap";
        long amount = -1;
        std::string protoc_filter;
        bool captured = false;
    };

    void init_listening(const config &config);

    std::vector<std::string> device_list();

    void pause_capturing();

    void console_handler(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    void file_handler(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    void ui_handler(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    void write_to_file(core::config &config, std::vector<pcap_pkthdr> &pkt_hdr, std::vector<const u_char *> &packet);
}

namespace sockets {
    struct table_view {
        std::string source;
        std::string destination;
        std::string protocol;
        unsigned int size;
        std::string info;
        std::string time; // TODO where to take
    };

    struct detail_view {
        std::string preview;
        std::vector<std::pair<std::string, std::string>> properties;
    };

    class base_socket {
    public:
        base_socket(u_char *args, const pcap_pkthdr *pkt_hdr, const u_char *packet);

        void print();

        std::string get_type() {
            return _get_type();
        };

        table_view to_row() {
            return _to_row();
        }

        std::vector<detail_view> to_view() {
            return _to_view();
        }

        const struct pcap_pkthdr *pkt_hdr;
        const u_char *packet;
        timeval ts;    /* time stamp */
        bpf_u_int32 caplen;    /* length of portion present */
        bpf_u_int32 len;
    private:
        virtual std::string _get_type() = 0;

        virtual table_view _to_row() = 0;

        virtual std::vector<detail_view> _to_view() = 0;
    };

    base_socket *parse_packet(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    class ethernet : public sockets::base_socket {
    public:

        ethernet(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    private:
        std::string _get_type() override;

        sockets::table_view _to_row() override;

        const struct ether_header *e_ptr;
    protected:
        std::vector<sockets::detail_view> _to_view() override;
    };

    class ip : public ethernet {
    public:

        ip(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    protected:
        std::vector<sockets::detail_view> _to_view() override;

        sockets::table_view _to_row() override;

    private:
        std::string _get_type() override;

        const struct sniff_ip *ip_ptr;

    };

    class tcp : public ip {
    public:

        tcp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    private:
        std::string _get_type() override;

        const struct sniff_tcp *tcp_ptr;

        std::vector<sockets::detail_view> _to_view() override;

        sockets::table_view _to_row() override;
    };

    class udp : public ip {
    public:

        udp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    private:
        std::string _get_type() override;

        const struct udp_header *udp_ptr;

        std::vector<sockets::detail_view> _to_view() override;

        sockets::table_view _to_row() override;
    };

    class arp : public ip {
    public:

        arp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    private:
        std::string _get_type() override;

        const struct arp_hdr *arp_ptr;

        std::vector<sockets::detail_view> _to_view() override;

        sockets::table_view _to_row() override;
    };
}

#endif //SNIFFER_CORE_H

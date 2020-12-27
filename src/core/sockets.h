// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_SOCKETS_H
#define SNIFFER_SOCKETS_H

#include "config.h"
#include <pcap.h>

namespace sockets {

    struct table_view {
        std::string source;
        std::string destination;
        std::string protocol;
        std::string info; // TODO add it
        unsigned int size;
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

        bool filter_size(int type, int length){
            switch (type - 1) {
                case Great:
                    return len > length;
                case Less:
                    return len < length;
                case Eaquals:
                    return len == length;
            }
            return false;
        }

        std::string source_layer(int type){
            return source_layer_(type);
        }
        std::string destination_layer(int type){
            return destination_layer_(type);
        };
        std::string protocol_layer(int type){
            return protocol_layer_(type);
        };

        const struct pcap_pkthdr *pkt_hdr;
        const u_char *packet;
        bpf_u_int32 cap_len;
        bpf_u_int32 len;
    private:
        virtual std::string _get_type() = 0;
        virtual table_view _to_row() = 0;
        virtual std::vector<detail_view> _to_view() = 0;
        virtual std::string source_layer_(int type) = 0;
        virtual std::string destination_layer_(int type) = 0;
        virtual std::string protocol_layer_(int type) = 0;
    };

    base_socket *parse_packet(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);


    ////// Begin types


    class ethernet : public sockets::base_socket {
    public:
        ethernet(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    protected:
        std::string _get_type() override;

        sockets::table_view _to_row() override;

        std::vector<sockets::detail_view> _to_view() override;

        std::string source_layer_(int type) override;
        std::string destination_layer_(int type) override;
        std::string protocol_layer_(int type) override;
    private:
        const struct ether_header *e_ptr;
    };

    class ip : public ethernet {
    public:
        ip(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    protected:
        std::vector<sockets::detail_view> _to_view() override;

        sockets::table_view _to_row() override;

        std::string _get_type() override;

        std::string source_layer_(int type) override;
        std::string destination_layer_(int type) override;
        std::string protocol_layer_(int type) override;
    private:
        const struct sniff_ip *ip_ptr;
    };

    class tcp : public ip {
    public:
        tcp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    protected:
        std::string _get_type() override;

        std::vector<sockets::detail_view> _to_view() override;

        sockets::table_view _to_row() override;

        std::string source_layer_(int type) override;
        std::string destination_layer_(int type) override;
        std::string protocol_layer_(int type) override;

    private:
        const struct sniff_tcp *tcp_ptr;
    };

    class udp : public ip {
    public:
        udp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    protected:
        std::string _get_type() override;

        std::vector<sockets::detail_view> _to_view() override;

        sockets::table_view _to_row() override;

        std::string source_layer_(int type) override;
        std::string destination_layer_(int type) override;
        std::string protocol_layer_(int type) override;

    private:
        const struct udp_header *udp_ptr;
    };

    class arp : public ip {
    public:
        arp(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    protected:
        std::string _get_type() override;

        std::vector<sockets::detail_view> _to_view() override;

        sockets::table_view _to_row() override;

        std::string source_layer_(int type) override;
        std::string destination_layer_(int type) override;
        std::string protocol_layer_(int type) override;

    private:
        const struct arp_hdr *arp_ptr;
    };
}
#endif //SNIFFER_SOCKETS_H
// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_CORE_H
#define SNIFFER_CORE_H

#include <pcap.h>
#include <string>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>

/* tcpdump header (ether.h) defines  */
#ifndef ETHER_HDRLEN
#define ETHER_HDRLEN 14
#endif

/* ethernet headers are always exactly 14 bytes */
#define SIZE_ETHERNET 14

/* IP header */

#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)

struct sniff_ip {
    u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
    u_char  ip_tos;                 /* type of service */
    u_short ip_len;                 /* total length */
    u_short ip_id;                  /* identification */
    u_short ip_off;                 /* fragment offset field */
#define IP_RF 0x8000            /* reserved fragment flag */
#define IP_DF 0x4000            /* don't fragment flag */
#define IP_MF 0x2000            /* more fragments flag */
#define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
    u_char  ip_ttl;                 /* time to live */
    u_char  ip_p;                   /* protocol */
    u_short ip_sum;                 /* checksum */
    struct  in_addr ip_src,ip_dst;  /* source and dest address */
};

typedef u_int tcp_seq;

struct sniff_tcp {
    u_short th_sport;               /* source port */
    u_short th_dport;               /* destination port */
    tcp_seq th_seq;                 /* sequence number */
    tcp_seq th_ack;                 /* acknowledgement number */
    u_char  th_offx2;               /* data offset, rsvd */
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
    u_char  th_flags;
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
#define TH_ECE  0x40
#define TH_CWR  0x80
#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;                 /* window */
    u_short th_sum;                 /* checksum */
    u_short th_urp;                 /* urgent pointer */
};


namespace core {
    struct config {
        bool to_file = false;
        bool from_file = false;
        bool both_out = false;
        std::string device = "wlp3s0"; // TODO
        std::string file_name = "log.pcap";
        std::string from_file_name = "log.pcap";
        int amount = -1;
        std::string protoc_filter;

    };

    void init_listening(const config& config);
    void print_from_file(const config& config);
}

namespace sockets {
    class base_socket {
    public:
        virtual void print() = 0;
        virtual std::string get_type() = 0;

        base_socket(u_char *args, const struct pcap_pkthdr* pkt_hdr, const u_char*packet): pkt_hdr(pkt_hdr), packet(packet){}

    protected:
        const struct pcap_pkthdr *pkt_hdr;
        const u_char *packet;
    };
    base_socket* parse_packet(u_char *args, const struct pcap_pkthdr* pkt_hdr, const u_char*packet);
    base_socket* parse_transport(u_char *args, const struct pcap_pkthdr* pkt_hdr, const u_char*packet, const std::string& type);
    base_socket* parse_network(u_char *args, const struct pcap_pkthdr* pkt_hdr, const u_char*packet, const std::string& type);



    class ethernet: public base_socket {
    public:
        void print() override;

        ethernet(u_char *args, const struct pcap_pkthdr* pkt_hdr, const u_char*packet);

        std::string get_type() override;
    private:
        const struct ether_header *e_ptr;
    };


    class ip: public ethernet{
    public:
        void print() override;

        ip(u_char *args, const struct pcap_pkthdr* pkt_hdr, const u_char*packet);

        std::string get_type() override;
    private:
        const struct sniff_ip* ip_ptr;
    };

    class tcp: public ip{
    public:
        void print() override;

        tcp(u_char *args, const struct pcap_pkthdr* pkt_hdr, const u_char*packet);

        std::string get_type() override;
    private:
        const struct sniff_tcp* tcp_ptr;
    };
}

#endif //SNIFFER_CORE_H

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

#include "config.h"
#include "core/structs.h"
#include "core/sockets.h"

namespace core {
    void control_handler(int s);
    class sniffer {
    public:

        void start_listen();

        void pause_listening();

        void close();

        explicit sniffer(config& config): configs(config){}
    protected:
        void init_listening_offline();
        void init_listening_online();

    private:
        config& configs;
        pcap_t *device = nullptr;
        bool is_listen = false;
        char err_buf[PCAP_ERRBUF_SIZE]{};
    };


    std::vector<std::string> device_list();

    void console_handler(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    void file_handler(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    void ui_handler(u_char *args, const struct pcap_pkthdr *pkt_hdr, const u_char *packet);

    void write_to_file(config &config, std::vector<pcap_pkthdr> &pkt_hdr, std::vector<const u_char *> &packet);
}

#endif //SNIFFER_CORE_H

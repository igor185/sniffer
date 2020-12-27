// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_UI_H
#define SNIFFER_UI_H

#include "core/core.h"


namespace UI {
    int init_gui(int argc, char** argv, config& config);
    void add_to_table(const struct pcap_pkthdr *pkt_hdr, const u_char *packet);
}
#endif //SNIFFER_UI_H

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_UDP_H
#define SNIFFER_UDP_H

struct udp_header {

    uint16_t udph_srcport;

    uint16_t udph_destport;

    uint16_t udph_len;

    uint16_t udph_chksum;

};

#endif //SNIFFER_UDP_H

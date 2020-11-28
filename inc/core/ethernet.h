// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_ETHERNET_H
#define SNIFFER_ETHERNET_H
#include <net/ethernet.h>

#include "core/core.h"

/* tcpdump header (ether.h) defines  */
#ifndef ETHER_HDRLEN
#define ETHER_HDRLEN 14
#endif

/* ethernet headers are always exactly 14 bytes */
#define SIZE_ETHERNET 14


#endif //SNIFFER_ETHERNET_H

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_CONFIG_H
#define SNIFFER_CONFIG_H

#include <string>
#include <vector>

#define FILE_RECENT_LOGS "recent.txt"

struct config {
    bool to_file = false;
    bool from_file = false;
    bool both_out = false; // TODO
    bool console = false;
    std::string device;
    std::string to_file_name = "log.pcap";
    std::string from_file_name = "log.pcap";
    int amount = -1;
    std::string protoc_filter;
    bool captured = false;
};

enum LengthType {
    Great = 0,
    Less,
    Eaquals
};

enum LayerType {
    Physic,
    Network,
    Transport
};

#endif //SNIFFER_CONFIG_H

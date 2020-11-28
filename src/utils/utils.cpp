// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <cstring>
#include <regex>
#include <unistd.h>

#include "util/utils.h"
#include "IO/IO.h"
#include "core/core.h"

bool utils::is_integer(const std::string &s) {
    return std::regex_match(s, std::regex("[0-9]+"));
}

int utils::to_number(const std::string &s) {
    return std::atoi(s.c_str());
}

core::config utils::cli_parser(int argc, char **argv) {
    core::config config = {};
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            IO::print_help();
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--device") == 0) {
            if (argc - 1 == i)
                IO::print_help();
            config.device = argv[i++];
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            if (argc - 1 == i)
                IO::print_help();
            config.to_file_name = argv[++i];
            config.to_file = true;
        } else if (strcmp(argv[i], "-ff") == 0 || strcmp(argv[i], "--from-file") == 0) {
            if (argc - 1 == i)
                IO::print_help();
            config.from_file_name = argv[++i];
            config.from_file = true;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--amount") == 0) {
            if (argc - 1 == i && !is_integer(argv[i] + 1))
                IO::print_help();
            config.amount = to_number(argv[++i]);
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0) {
            config.both_out = true;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--protocol") == 0) {
            if (argc - 1 == i)
                IO::print_help();
            config.protoc_filter = argv[++i];
        }
    }
    return config;
}

std::string utils::get_time(const timeval &ts) {
    char buf[20];

    struct tm *gm = gmtime(&ts.tv_sec);

    strftime(buf, sizeof(buf), "%H:%M:%S", gm);
    return buf;
}


/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 */
void
print_hex_ascii_line(const u_char *payload, int len, int offset, bool print_number = true) {

    int i;
    int gap;
    const u_char *ch;

    /* offset */
    printf("%05d   ", offset);

    /* hex */
    ch = payload;
    for (i = 0; i < len; i++) {
        printf("%02x ", *ch);
        ch++;
        /* print extra space after 8th byte for visual aid */
        if (i == 7)
            printf(" ");
    }
    /* print space to handle line less than 8 bytes */
    if (len < 8)
        printf(" ");

    /* fill hex gap with spaces if not full line */
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            printf("   ");
        }
    }
    printf("   ");

    /* ascii (if printable) */
    ch = payload;
    for (i = 0; i < len; i++) {
        if (isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }

    printf("\n");

}

/*
 * print packet payload data (avoid printing binary data)
 */
void
utils::print_payload(const u_char *payload, int len) {

    int len_rem = len;
    int line_width = 16;            /* number of bytes per line */
    int line_len;
    int offset = 0;                    /* zero-based offset counter */
    const u_char *ch = payload;

    if (len <= 0)
        return;

    /* data fits on one line */
    if (len <= line_width) {
        print_hex_ascii_line(ch, len, offset);
        return;
    }

    /* data spans multiple lines */
    for (;;) {
        /* compute current line length */
        line_len = line_width % len_rem;
        /* print line */
        print_hex_ascii_line(ch, line_len, offset);
        /* compute total remaining */
        len_rem = len_rem - line_len;
        /* shift pointer to remaining bytes to print */
        ch = ch + line_len;
        /* add offset */
        offset = offset + line_width;
        /* check if we have line width chars or less */
        if (len_rem <= line_width) {
            /* print last line and get out */
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }
}


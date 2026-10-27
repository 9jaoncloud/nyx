/*
 * rt_pcap.c — Nyx Runtime Packet Crafting & NIDS Inspection Implementation
 */

#include "rt_pcap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

uint16_t nyx_ip_checksum(const void* buf, size_t len) {
    const uint16_t* ptr = (const uint16_t*)buf;
    uint32_t sum = 0;
    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }
    if (len == 1) {
        sum += *(const uint8_t*)ptr;
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (uint16_t)(~sum);
}

size_t nyx_craft_syn_packet(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t* out_pkt, size_t max_len) {
    size_t total_size = sizeof(NyxEthHeader) + sizeof(NyxIpHeader) + sizeof(NyxTcpHeader);
    if (!out_pkt || max_len < total_size) return 0;

    memset(out_pkt, 0, total_size);

    /* 1. Ethernet Header */
    NyxEthHeader* eth = (NyxEthHeader*)out_pkt;
    memset(eth->dst_mac, 0xFF, 6); /* Broadcast MAC */
    eth->src_mac[0] = 0x00; eth->src_mac[1] = 0x1A; eth->src_mac[2] = 0x2B;
    eth->src_mac[3] = 0x3C; eth->src_mac[4] = 0x4D; eth->src_mac[5] = 0x5E;
    eth->ethertype = htons(0x0800); /* IPv4 */

    /* 2. IPv4 Header */
    NyxIpHeader* ip = (NyxIpHeader*)(out_pkt + sizeof(NyxEthHeader));
    ip->ver_ihl = 0x45; /* IPv4, 5 x 32-bit words */
    ip->tos = 0;
    ip->total_len = htons((uint16_t)(sizeof(NyxIpHeader) + sizeof(NyxTcpHeader)));
    ip->id = htons(54321);
    ip->flags_frag = 0;
    ip->ttl = 64;
    ip->proto = 6; /* TCP */
    ip->src_ip = src_ip;
    ip->dst_ip = dst_ip;
    ip->checksum = nyx_ip_checksum(ip, sizeof(NyxIpHeader));

    /* 3. TCP SYN Header */
    NyxTcpHeader* tcp = (NyxTcpHeader*)(out_pkt + sizeof(NyxEthHeader) + sizeof(NyxIpHeader));
    tcp->src_port = htons(src_port);
    tcp->dst_port = htons(dst_port);
    tcp->seq_num = htonl(1000001);
    tcp->ack_num = 0;
    tcp->data_offset = 0x50; /* 5 words header */
    tcp->flags = 0x02;       /* SYN Flag */
    tcp->window_size = htons(65535);
    tcp->checksum = htons(0xCAFE); /* Computed by hardware NIC offload */
    tcp->urgent_ptr = 0;

    return total_size;
}

bool nyx_inspect_packet_signature(const uint8_t* pkt, size_t len, char* out_alert, size_t alert_len) {
    if (!pkt || len < sizeof(NyxEthHeader) + sizeof(NyxIpHeader)) return false;

    const NyxEthHeader* eth = (const NyxEthHeader*)pkt;
    if (ntohs(eth->ethertype) != 0x0800) return false;

    const NyxIpHeader* ip = (const NyxIpHeader*)(pkt + sizeof(NyxEthHeader));
    if (ip->proto == 6 && len >= sizeof(NyxEthHeader) + sizeof(NyxIpHeader) + sizeof(NyxTcpHeader)) {
        const NyxTcpHeader* tcp = (const NyxTcpHeader*)(pkt + sizeof(NyxEthHeader) + sizeof(NyxIpHeader));
        uint8_t flags = tcp->flags;

        /* Check for SYN-FIN or NULL Scan anomaly */
        if ((flags & 0x03) == 0x03) { /* SYN + FIN active */
            if (out_alert && alert_len > 0) {
                snprintf(out_alert, alert_len, "ALERT [NIDS-001]: Anomalous TCP SYN+FIN Stealth Scan Detected from Port %d", ntohs(tcp->src_port));
            }
            return true;
        }
        if (flags == 0x00) { /* NULL Scan */
            if (out_alert && alert_len > 0) {
                snprintf(out_alert, alert_len, "ALERT [NIDS-002]: Anomalous TCP NULL Scan Detected from Port %d", ntohs(tcp->src_port));
            }
            return true;
        }
    }
    return false;
}

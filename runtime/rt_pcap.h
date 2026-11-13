/*
 * rt_pcap.h — Nyx Runtime Raw Packet Crafting, PCAP Analysis & Network Defense Engine
 *
 * Implements Scapy-like packet construction (Ethernet, IPv4, TCP, UDP, ICMP)
 * with fast checksum calculation, PCAP binary headers, and NIDS signature matching.
 */

#ifndef RT_PCAP_H
#define RT_PCAP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Ethernet Frame (14 bytes) */
typedef struct {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype; /* 0x0800 for IPv4 */
} __attribute__((packed)) NyxEthHeader;

/* IPv4 Header (20 bytes) */
typedef struct {
    uint8_t ver_ihl;      /* Version (4 bits) + IHL (4 bits) */
    uint8_t tos;
    uint16_t total_len;
    uint16_t id;
    uint16_t flags_frag;
    uint8_t ttl;
    uint8_t proto;        /* 6 for TCP, 17 for UDP, 1 for ICMP */
    uint16_t checksum;
    uint32_t src_ip;
    uint32_t dst_ip;
} __attribute__((packed)) NyxIpHeader;

/* TCP Header (20 bytes) */
typedef struct {
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq_num;
    uint32_t ack_num;
    uint8_t data_offset;  /* 5 << 4 */
    uint8_t flags;        /* SYN, ACK, FIN, RST, PSH, URG */
    uint16_t window_size;
    uint16_t checksum;
    uint16_t urgent_ptr;
} __attribute__((packed)) NyxTcpHeader;

/* Packet Crafting API */
uint16_t nyx_ip_checksum(const void* buf, size_t len);
size_t nyx_craft_syn_packet(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t* out_pkt, size_t max_len);
bool nyx_inspect_packet_signature(const uint8_t* pkt, size_t len, char* out_alert, size_t alert_len);

#ifdef __cplusplus
}
#endif

#endif /* RT_PCAP_H */

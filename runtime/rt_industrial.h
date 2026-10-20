/*
 * rt_industrial.h — Nyx Universal Industrial Automation & SCADA Suite
 *
 * Implements:
 *  1. Modbus TCP & RTU: Read/Write Holding Registers & Coils
 *  2. IEC 61850 GOOSE: Sub-millisecond Layer-2 Ethernet Protection Frame Encoder
 *  3. DNP3 (Distributed Network Protocol): Industrial SCADA Analog/Binary Input Framer
 */

#ifndef RT_INDUSTRIAL_H
#define RT_INDUSTRIAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Modbus TCP Protocol ───────────────────────────────────────────────── */
typedef struct {
    uint16_t transaction_id;
    uint16_t protocol_id;       /* Always 0x0000 for Modbus TCP */
    uint16_t length;
    uint8_t unit_id;
    uint8_t function_code;      /* e.g. 0x03 Read Holding Registers, 0x06 Write Single Register */
    uint16_t start_address;
    uint16_t register_count;
} NyxModbusTcpRequest;

typedef struct {
    uint16_t transaction_id;
    uint8_t unit_id;
    uint8_t function_code;
    uint8_t byte_count;
    uint16_t registers[16];
} NyxModbusTcpResponse;

size_t nyx_modbus_encode_read_holding_registers(uint16_t tx_id, uint8_t unit_id, uint16_t addr, uint16_t count, uint8_t* out_buf);
NyxModbusTcpResponse nyx_modbus_decode_response(const uint8_t* in_buf, size_t len);

/* ── 2. IEC 61850 GOOSE Frame (Generic Object Oriented Substation Events) ── */
typedef struct {
    uint8_t dst_mac[6];         /* Multicast 01:0C:CD:01:00:xx */
    uint8_t src_mac[6];
    uint16_t ethertype;         /* 0x88B8 for IEC 61850 GOOSE */
    uint16_t app_id;            /* Application ID (0x0000 - 0x3FFF) */
    char go_cb_ref[64];         /* Control Block Reference e.g. "SUB01_LD0/LLN0$GO$gcb01" */
    char dat_set[64];           /* Dataset Reference e.g. "SUB01_LD0/LLN0$DS_TRIP" */
    char go_id[32];             /* GOOSE ID */
    uint32_t state_num;         /* stNum: Increment on state change */
    uint32_t seq_num;           /* sqNum: Increment on heartbeat retransmission */
    bool trip_circuit_breaker;  /* Critical boolean value */
} NyxIec61850GooseFrame;

size_t nyx_iec61850_encode_goose_frame(const NyxIec61850GooseFrame* frame, uint8_t* out_ethernet_frame, size_t max_len);

/* ── 3. DNP3 SCADA Protocol ──────────────────────────────────────────────── */
typedef struct {
    uint16_t source_address;
    uint16_t destination_address;
    uint8_t function_code;      /* 0x01 Read, 0x81 Response */
    double analog_value;        /* e.g. MW or Voltage KV */
    bool binary_alarm_active;
} NyxDnp3TelemetryPacket;

size_t nyx_dnp3_encode_telemetry(const NyxDnp3TelemetryPacket* packet, uint8_t* out_buf);

#ifdef __cplusplus
}
#endif

#endif /* RT_INDUSTRIAL_H */

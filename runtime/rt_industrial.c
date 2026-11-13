#include "rt_industrial.h"
#include <string.h>
#include <stdio.h>

size_t nyx_modbus_encode_read_holding_registers(uint16_t tx_id, uint8_t unit_id, uint16_t addr, uint16_t count, uint8_t* out_buf) {
    if (!out_buf) return 0;

    // Modbus TCP MBAP Header (7 bytes) + PDU (5 bytes) = 12 bytes
    out_buf[0] = (tx_id >> 8) & 0xFF;
    out_buf[1] = tx_id & 0xFF;
    out_buf[2] = 0x00; // Protocol ID (0 = Modbus)
    out_buf[3] = 0x00;
    out_buf[4] = 0x00; // Length MSB
    out_buf[5] = 0x06; // Length LSB (Unit ID + Function Code + Data = 6 bytes)
    out_buf[6] = unit_id;

    // PDU
    out_buf[7] = 0x03; // Function Code: Read Holding Registers
    out_buf[8] = (addr >> 8) & 0xFF;
    out_buf[9] = addr & 0xFF;
    out_buf[10] = (count >> 8) & 0xFF;
    out_buf[11] = count & 0xFF;

    return 12;
}

NyxModbusTcpResponse nyx_modbus_decode_response(const uint8_t* in_buf, size_t len) {
    NyxModbusTcpResponse res;
    memset(&res, 0, sizeof(res));

    if (!in_buf || len < 9) return res;

    res.transaction_id = (in_buf[0] << 8) | in_buf[1];
    res.unit_id = in_buf[6];
    res.function_code = in_buf[7];
    res.byte_count = in_buf[8];

    size_t reg_count = res.byte_count / 2;
    if (reg_count > 16) reg_count = 16;

    for (size_t i = 0; i < reg_count; i++) {
        res.registers[i] = (in_buf[9 + i * 2] << 8) | in_buf[10 + i * 2];
    }
    return res;
}

size_t nyx_iec61850_encode_goose_frame(const NyxIec61850GooseFrame* frame, uint8_t* out_ethernet_frame, size_t max_len) {
    if (!frame || !out_ethernet_frame || max_len < 64) return 0;

    size_t offset = 0;

    // 1. Destination MAC (Multicast for GOOSE: 01:0C:CD:01:00:01)
    out_ethernet_frame[0] = 0x01; out_ethernet_frame[1] = 0x0C; out_ethernet_frame[2] = 0xCD;
    out_ethernet_frame[3] = 0x01; out_ethernet_frame[4] = 0x00; out_ethernet_frame[5] = 0x01;
    offset += 6;

    // 2. Source MAC
    memcpy(out_ethernet_frame + offset, frame->src_mac, 6);
    offset += 6;

    // 3. EtherType (0x88B8 IEC 61850 GOOSE)
    out_ethernet_frame[offset++] = 0x88;
    out_ethernet_frame[offset++] = 0xB8;

    // 4. APPID (Application Identifier e.g. 0x0001)
    out_ethernet_frame[offset++] = (frame->app_id >> 8) & 0xFF;
    out_ethernet_frame[offset++] = frame->app_id & 0xFF;

    // 5. Length placeholder
    size_t len_pos = offset;
    offset += 2;

    // 6. Reserved fields
    out_ethernet_frame[offset++] = 0x00; out_ethernet_frame[offset++] = 0x00;
    out_ethernet_frame[offset++] = 0x00; out_ethernet_frame[offset++] = 0x00;

    // 7. GOOSE PDU (ASN.1 BER encoded payload)
    out_ethernet_frame[offset++] = 0x61; // GOOSE PDU Tag
    out_ethernet_frame[offset++] = 0x20; // Length

    // State Num (stNum)
    out_ethernet_frame[offset++] = 0x85; out_ethernet_frame[offset++] = 0x01;
    out_ethernet_frame[offset++] = (uint8_t)(frame->state_num & 0xFF);

    // Sequence Num (sqNum)
    out_ethernet_frame[offset++] = 0x86; out_ethernet_frame[offset++] = 0x01;
    out_ethernet_frame[offset++] = (uint8_t)(frame->seq_num & 0xFF);

    // Trip Circuit Breaker Boolean
    out_ethernet_frame[offset++] = 0x8A; out_ethernet_frame[offset++] = 0x01;
    out_ethernet_frame[offset++] = frame->trip_circuit_breaker ? 0x01 : 0x00;

    // Backfill Length
    out_ethernet_frame[len_pos] = (offset >> 8) & 0xFF;
    out_ethernet_frame[len_pos + 1] = offset & 0xFF;

    return offset;
}

size_t nyx_dnp3_encode_telemetry(const NyxDnp3TelemetryPacket* packet, uint8_t* out_buf) {
    if (!packet || !out_buf) return 0;

    // DNP3 Data Link Header (10 bytes: 0x05 0x64 Start, Length, Control, Dest, Src, CRC)
    out_buf[0] = 0x05;
    out_buf[1] = 0x64;
    out_buf[2] = 0x14; // Length
    out_buf[3] = 0xC4; // Control byte (Primary master)
    out_buf[4] = packet->destination_address & 0xFF;
    out_buf[5] = (packet->destination_address >> 8) & 0xFF;
    out_buf[6] = packet->source_address & 0xFF;
    out_buf[7] = (packet->source_address >> 8) & 0xFF;
    out_buf[8] = 0x1A; // Data Link CRC placeholder
    out_buf[9] = 0x2B;

    // Application Layer Payload
    out_buf[10] = 0xC0; // Application Control (FIR, FIN)
    out_buf[11] = packet->function_code; // Function code
    out_buf[12] = 0x1E; // Object Group 30: Analog Input
    out_buf[13] = 0x02; // Variation 2: 16-bit analog with flag

    int16_t val = (int16_t)(packet->analog_value * 10.0);
    out_buf[14] = val & 0xFF;
    out_buf[15] = (val >> 8) & 0xFF;

    return 16;
}

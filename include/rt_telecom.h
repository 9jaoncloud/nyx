/*
 * rt_telecom.h — Nyx Universal Telecommunications, Radio Decoders & Emergency Broadcast
 *
 * Implements:
 *  1. IMO Maritime AIS (Automatic Identification System) NMEA 0183 6-bit Binary Payload Decoder
 *  2. Aviation ADS-B Mode-S Extended Squitter (1090 MHz) Transponder Decoder
 *  3. OASIS Common Alerting Protocol (CAP v1.2) XML & 3GPP Cell Broadcast (CBS) Formatter
 */

#ifndef RT_TELECOM_H
#define RT_TELECOM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Maritime AIS NMEA 0183 (Automatic Identification System) ─────────── */
typedef struct {
    uint8_t message_type;       /* 1, 2, 3 = Position Report Class A */
    uint32_t mmsi;              /* Maritime Mobile Service Identity (9-digit unique vessel ID) */
    uint8_t navigational_status;/* 0 = Under way using engine, 1 = At anchor, 5 = Moored */
    double speed_over_ground_knots;
    double longitude;           /* WGS84 decimal degrees (-180.0 to +180.0) */
    double latitude;            /* WGS84 decimal degrees (-90.0 to +90.0) */
    double course_over_ground_deg;
    uint16_t true_heading_deg;
    bool is_valid;
} NyxMmsiAisPositionReport;

NyxMmsiAisPositionReport nyx_telecom_decode_ais_type1(const char* nmea_payload_6bit);

/* ── 2. Aviation ADS-B Mode-S Extended Squitter (1090 MHz) ───────────────── */
typedef struct {
    uint32_t icao_24bit_address;/* 24-bit ICAO aircraft transponder address (hex e.g. 0x06A14B) */
    uint8_t type_code;          /* 9-18 = Airborne Position, 19 = Airborne Velocity */
    int32_t altitude_feet;
    double speed_knots;
    double heading_deg;
    uint16_t squawk_code;       /* 7700 = Emergency, 7600 = Radio Fail, 7500 = Hijack */
    bool is_emergency;
} NyxAdsbAircraftState;

NyxAdsbAircraftState nyx_telecom_decode_adsb_mode_s(const uint8_t* raw_14byte_msg);

/* ── 3. OASIS Common Alerting Protocol (CAP v1.2) ────────────────────────── */
typedef enum {
    CAP_SEVERITY_MINOR,
    CAP_SEVERITY_MODERATE,
    CAP_SEVERITY_SEVERE,
    CAP_SEVERITY_EXTREME
} NyxCapSeverity;

typedef struct {
    char identifier[64];
    char sender[64];
    char headline[128];
    char description[256];
    char area_polygon[128];     /* "lat,lon lat,lon ..." */
    NyxCapSeverity severity;
} NyxCapAlert;

char* nyx_telecom_generate_cap_xml(const NyxCapAlert* alert);
size_t nyx_telecom_encode_3gpp_cbs(const NyxCapAlert* alert, uint8_t* out_cbs_payload, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* RT_TELECOM_H */

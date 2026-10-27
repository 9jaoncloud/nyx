#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ── Nigeria NIPOST 6-Digit Validation & Decoding ── */
bool rt_postcode_validate_ng(const char* code) {
    if (!code) return false;
    size_t len = strlen(code);
    if (len != 6) return false;
    for (size_t i = 0; i < 6; i++) {
        if (code[i] < '0' || code[i] > '9') return false;
    }
    // Digit 1 must be 1..9
    if (code[0] < '1' || code[0] > '9') return false;
    return true;
}

int rt_postcode_get_zone_ng(const char* code) {
    if (!rt_postcode_validate_ng(code)) return 0;
    return code[0] - '0';
}

const char* rt_postcode_get_state_ng(const char* code) {
    if (!rt_postcode_validate_ng(code)) return "Invalid Code";
    int prefix = (code[0] - '0') * 10 + (code[1] - '0');
    
    // NIPOST 2-digit prefix state mapping
    if (prefix >= 10 && prefix <= 11) return "Lagos";
    if (prefix == 12) return "Ogun";
    if (prefix >= 20 && prefix <= 22) return "Oyo";
    if (prefix == 23) return "Osun";
    if (prefix == 24) return "Kwara";
    if (prefix >= 30 && prefix <= 31) return "Edo";
    if (prefix >= 32 && prefix <= 33) return "Delta";
    if (prefix >= 34 && prefix <= 35) return "Ondo / Ekiti";
    if (prefix >= 40 && prefix <= 41) return "Enugu";
    if (prefix >= 42 && prefix <= 43) return "Anambra";
    if (prefix == 44) return "Abia";
    if (prefix == 46) return "Imo";
    if (prefix == 48) return "Ebonyi";
    if (prefix >= 50 && prefix <= 51) return "Rivers";
    if (prefix == 52) return "Akwa Ibom";
    if (prefix == 54) return "Cross River";
    if (prefix == 56) return "Bayelsa";
    if (prefix >= 70 && prefix <= 71) return "Kano";
    if (prefix == 72) return "Jigawa";
    if (prefix == 73) return "Katsina";
    if (prefix >= 80 && prefix <= 81) return "Kaduna";
    if (prefix == 82) return "Sokoto";
    if (prefix == 83) return "Kebbi";
    if (prefix == 84) return "Zamfara";
    if (prefix == 86) return "Niger";
    if (prefix == 90) return "Abuja (FCT)";
    if (prefix == 91) return "Nasarawa";
    if (prefix == 92) return "Niger";
    if (prefix == 93) return "Plateau";
    if (prefix == 94) return "Benue";
    if (prefix == 95) return "Taraba";
    if (prefix == 96) return "Adamawa";
    if (prefix == 97) return "Bauchi / Gombe";
    if (prefix == 98) return "Borno / Yobe";
    
    return "Nigeria (Regional Zone)";
}

/* ── Global UPU Validation (Universal Postal Union ISO Standards) ── */
bool rt_postcode_validate_upu(const char* country_iso, const char* code) {
    if (!country_iso || !code) return false;
    size_t len = strlen(code);
    if (len == 0) return false;

    if (strcmp(country_iso, "NG") == 0) {
        return rt_postcode_validate_ng(code);
    }
    if (strcmp(country_iso, "US") == 0) {
        // 5 digits or 5+4 digits
        if (len == 5) {
            for (size_t i = 0; i < 5; i++) if (code[i] < '0' || code[i] > '9') return false;
            return true;
        }
        if (len == 10 && code[5] == '-') {
            for (size_t i = 0; i < 5; i++) if (code[i] < '0' || code[i] > '9') return false;
            for (size_t i = 6; i < 10; i++) if (code[i] < '0' || code[i] > '9') return false;
            return true;
        }
        return false;
    }
    if (strcmp(country_iso, "GB") == 0 || strcmp(country_iso, "UK") == 0) {
        // UK alphanumeric standard 5-8 chars
        return len >= 5 && len <= 8;
    }
    if (strcmp(country_iso, "CA") == 0) {
        // A1A 1A1
        return len >= 6 && len <= 7;
    }
    if (strcmp(country_iso, "DE") == 0 || strcmp(country_iso, "FR") == 0 || strcmp(country_iso, "ES") == 0 || strcmp(country_iso, "IT") == 0) {
        // 5 digits
        if (len != 5) return false;
        for (size_t i = 0; i < 5; i++) if (code[i] < '0' || code[i] > '9') return false;
        return true;
    }
    if (strcmp(country_iso, "JP") == 0) {
        // 7 digits or 3-4
        return (len == 7 || (len == 8 && code[3] == '-'));
    }

    // Default global sanity check
    return len >= 3 && len <= 12;
}

/* ── Haversine Geodesic Distance (km) ── */
static double to_rad(double degree) {
    return degree * (M_PI / 180.0);
}

double rt_postcode_distance_km(double lat1, double lon1, double lat2, double lon2) {
    double earth_radius = 6371.0; // Mean Earth radius in km
    double d_lat = to_rad(lat2 - lat1);
    double d_lon = to_rad(lon2 - lon1);
    
    double a = sin(d_lat / 2.0) * sin(d_lat / 2.0) +
               cos(to_rad(lat1)) * cos(to_rad(lat2)) *
               sin(d_lon / 2.0) * sin(d_lon / 2.0);
               
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return earth_radius * c;
}

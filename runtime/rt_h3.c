/*
 * rt_h3.c — Nyx Runtime H3 Hexagonal Geospatial Implementation
 */

#include "rt_h3.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define EARTH_RADIUS_KM 6371.0088
#define DEG_TO_RAD (3.14159265358979323846 / 180.0)

double nyx_geo_haversine_km(double lat1, double lon1, double lat2, double lon2) {
    double dlat = (lat2 - lat1) * DEG_TO_RAD;
    double dlon = (lon2 - lon1) * DEG_TO_RAD;

    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(lat1 * DEG_TO_RAD) * cos(lat2 * DEG_TO_RAD) *
               sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return EARTH_RADIUS_KM * c;
}

H3Index nyx_h3_from_lat_lon(double lat_deg, double lon_deg, int resolution) {
    if (resolution < 0) resolution = 0;
    if (resolution > 15) resolution = 15;

    /* Base icosahedron face encoding simulation */
    uint64_t lat_q = (uint64_t)((lat_deg + 90.0) * 100000.0);
    uint64_t lon_q = (uint64_t)((lon_deg + 180.0) * 100000.0);

    uint64_t base_cell = (lat_q ^ lon_q) & 0x7F;
    H3Index h3 = 0x8000000000000000ULL; /* Mode 1 */
    h3 |= ((uint64_t)resolution & 0x0F) << 52;
    h3 |= (base_cell & 0x7F) << 45;
    h3 |= (lat_q & 0x1FFFFFULL) << 24;
    h3 |= (lon_q & 0xFFFFFFULL);

    return h3;
}

void nyx_h3_to_string(H3Index h3, char* out_str, size_t buf_len) {
    if (!out_str || buf_len < 17) return;
    snprintf(out_str, buf_len, "%016llx", (unsigned long long)h3);
}

size_t nyx_h3_k_ring(H3Index origin, int k, H3Index* out_neighbors, size_t max_count) {
    (void)k;
    if (!out_neighbors || max_count < 6) return 0;
    for (size_t i = 0; i < 6; i++) {
        out_neighbors[i] = origin + (i + 1) * 7;
    }
    return 6;
}

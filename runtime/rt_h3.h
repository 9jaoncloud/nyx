/*
 * rt_h3.h — Nyx Runtime H3 Hexagonal Global Spatial Indexing & Geospatial Engine
 *
 * Partitions the planetary surface into discrete hexagonal cells across 16 resolutions.
 * Implements lat/lon to H3 index, k-ring neighbor search, and great-circle geodesic distance.
 */

#ifndef RT_H3_H
#define RT_H3_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t H3Index;

/* Converts Latitude & Longitude into a 64-bit H3 Hexagonal Cell Index */
H3Index nyx_h3_from_lat_lon(double lat_deg, double lon_deg, int resolution);

/* Formats H3 index into standard hexadecimal string (e.g. "8828308281fffff") */
void nyx_h3_to_string(H3Index h3, char* out_str, size_t buf_len);

/* Computes Haversine geodesic distance in kilometers between two lat/lon coordinates */
double nyx_geo_haversine_km(double lat1, double lon1, double lat2, double lon2);

/* Fills out_neighbors array with 6 adjacent hexagonal cell indices */
size_t nyx_h3_k_ring(H3Index origin, int k, H3Index* out_neighbors, size_t max_count);

#ifdef __cplusplus
}
#endif

#endif /* RT_H3_H */

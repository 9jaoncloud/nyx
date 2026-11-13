#ifndef RT_GIS_H
#define RT_GIS_H

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// WGS84 Geodesic Distance in Kilometers (Haversine Formula)
static inline double rt_gis_distance_km(double lat1, double lon1, double lat2, double lon2) {
    double r = 6371.0;
    double dlat = (lat2 - lat1) * (M_PI / 180.0);
    double dlon = (lon2 - lon1) * (M_PI / 180.0);
    double l1 = lat1 * (M_PI / 180.0);
    double l2 = lat2 * (M_PI / 180.0);

    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(l1) * cos(l2) * sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return r * c;
}

// Forward Web Mercator Projection (WGS84 EPSG:4326 -> EPSG:3857) X coordinate
static inline double rt_gis_web_mercator_x(double lon) {
    double r_major = 6378137.0;
    return r_major * (lon * M_PI / 180.0);
}

// Forward Web Mercator Projection Y coordinate
static inline double rt_gis_web_mercator_y(double lat) {
    double r_major = 6378137.0;
    double scale = sin(lat * M_PI / 180.0);
    return 0.5 * r_major * log((1.0 + scale) / (1.0 - scale));
}

// Inverse Web Mercator Projection (EPSG:3857 -> WGS84 EPSG:4326) Latitude
static inline double rt_gis_wgs84_lat(double x, double y) {
    (void)x;
    double r_major = 6378137.0;
    return (2.0 * atan(exp(y / r_major)) - (M_PI / 2.0)) * (180.0 / M_PI);
}

// Inverse Web Mercator Projection Longitude
static inline double rt_gis_wgs84_lon(double x, double y) {
    (void)y;
    double r_major = 6378137.0;
    return (x / r_major) * (180.0 / M_PI);
}

// Raster Hillshade Illumination Calculation (Sun Azimuth: 315, Altitude: 45)
static inline double rt_gis_hillshade(double dz_dx, double dz_dy) {
    double zenith_rad = (90.0 - 45.0) * (M_PI / 180.0);
    double azimuth_rad = 315.0 * (M_PI / 180.0);
    double slope_rad = atan(sqrt(dz_dx * dz_dx + dz_dy * dz_dy));
    double aspect_rad = atan2(dz_dy, -dz_dx);

    double hillshade = (cos(zenith_rad) * cos(slope_rad)) +
                        (sin(zenith_rad) * sin(slope_rad) * cos(azimuth_rad - aspect_rad));
    return hillshade < 0.0 ? 0.0 : hillshade;
}

#endif // RT_GIS_H

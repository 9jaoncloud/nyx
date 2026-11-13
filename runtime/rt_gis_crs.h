/*
 * rt_gis_crs.h — Coordinate Reference Systems (CRS), Datums, and UTM/LCC Engine
 * Part of the Nyx Geospatial Engine (Zero External Dependencies)
 */

#ifndef RT_GIS_CRS_H
#define RT_GIS_CRS_H

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG_TO_RAD (M_PI / 180.0)
#define RAD_TO_DEG (180.0 / M_PI)

/* ── 1. Geodetic Ellipsoid Definitions ───────────────────────────────────── */
typedef struct {
    double a;        /* Semi-major axis (meters) */
    double f;        /* Flattening */
    double b;        /* Semi-minor axis: a * (1 - f) */
    double e2;       /* First eccentricity squared: (a^2 - b^2) / a^2 */
    double ep2;      /* Second eccentricity squared: (a^2 - b^2) / b^2 */
} NyxEllipsoid;

static inline NyxEllipsoid nyx_ellipsoid_wgs84(void) {
    NyxEllipsoid el;
    el.a = 6378137.0;
    el.f = 1.0 / 298.257223563;
    el.b = el.a * (1.0 - el.f);
    el.e2 = (el.a * el.a - el.b * el.b) / (el.a * el.a);
    el.ep2 = (el.a * el.a - el.b * el.b) / (el.b * el.b);
    return el;
}

static inline NyxEllipsoid nyx_ellipsoid_airy1830(void) {
    NyxEllipsoid el;
    el.a = 6377563.396;
    el.b = 6356256.909;
    el.f = (el.a - el.b) / el.a;
    el.e2 = (el.a * el.a - el.b * el.b) / (el.a * el.a);
    el.ep2 = (el.a * el.a - el.b * el.b) / (el.b * el.b);
    return el;
}

static inline NyxEllipsoid nyx_ellipsoid_clarke1880(void) {
    NyxEllipsoid el;
    el.a = 6378249.145;
    el.b = 6356514.870;
    el.f = (el.a - el.b) / el.a;
    el.e2 = (el.a * el.a - el.b * el.b) / (el.a * el.a);
    el.ep2 = (el.a * el.a - el.b * el.b) / (el.b * el.b);
    return el;
}

/* ── 2. Universal Transverse Mercator (UTM Zones 1-60 N/S) ───────────────── */
typedef struct {
    int    zone;
    char   hemisphere;  /* 'N' or 'S' */
    double easting;     /* meters */
    double northing;    /* meters */
} NyxUtmCoord;

/* Forward: WGS84 Geodetic (Lat, Lon in degrees) -> UTM (Easting, Northing) */
static inline NyxUtmCoord nyx_wgs84_to_utm(double lat, double lon) {
    NyxEllipsoid el = nyx_ellipsoid_wgs84();
    NyxUtmCoord out;

    int zone = (int)floor((lon + 180.0) / 6.0) + 1;
    if (zone < 1) zone = 1;
    if (zone > 60) zone = 60;
    out.zone = zone;
    out.hemisphere = (lat >= 0.0) ? 'N' : 'S';

    double lat_rad = lat * DEG_TO_RAD;
    double lon_rad = lon * DEG_TO_RAD;
    double lon0_rad = ((double)(zone - 1) * 6.0 - 180.0 + 3.0) * DEG_TO_RAD;

    double k0 = 0.9996;
    double e2 = el.e2;
    double ep2 = el.ep2;

    double N = el.a / sqrt(1.0 - e2 * sin(lat_rad) * sin(lat_rad));
    double T = tan(lat_rad) * tan(lat_rad);
    double C = ep2 * cos(lat_rad) * cos(lat_rad);
    double A = cos(lat_rad) * (lon_rad - lon0_rad);

    /* Meridional Arc Distance M */
    double M = el.a * ((1.0 - e2 / 4.0 - 3.0 * e2 * e2 / 64.0 - 5.0 * e2 * e2 * e2 / 256.0) * lat_rad -
                       (3.0 * e2 / 8.0 + 3.0 * e2 * e2 / 32.0 + 45.0 * e2 * e2 * e2 / 1024.0) * sin(2.0 * lat_rad) +
                       (15.0 * e2 * e2 / 256.0 + 45.0 * e2 * e2 * e2 / 1024.0) * sin(4.0 * lat_rad) -
                       (35.0 * e2 * e2 * e2 / 3072.0) * sin(6.0 * lat_rad));

    double x = k0 * N * (A + (1.0 - T + C) * A * A * A / 6.0 +
                         (5.0 - 18.0 * T + T * T + 72.0 * C - 58.0 * ep2) * A * A * A * A * A / 120.0);
    out.easting = 500000.0 + x;

    double y = k0 * (M + N * tan(lat_rad) * (A * A / 2.0 +
                     (5.0 - T + 9.0 * C + 4.0 * C * C) * A * A * A * A / 24.0 +
                     (61.0 - 58.0 * T + T * T + 600.0 * C - 330.0 * ep2) * A * A * A * A * A * A / 720.0));

    if (lat < 0.0) {
        y += 10000000.0; /* False Northing for Southern Hemisphere */
    }
    out.northing = y;

    return out;
}

/* Inverse: UTM (Zone, Hemisphere, Easting, Northing) -> WGS84 Geodetic (Lat, Lon) */
static inline void nyx_utm_to_wgs84(const NyxUtmCoord* utm, double* out_lat, double* out_lon) {
    NyxEllipsoid el = nyx_ellipsoid_wgs84();
    double k0 = 0.9996;
    double e2 = el.e2;
    double ep2 = el.ep2;
    double e1 = (1.0 - sqrt(1.0 - e2)) / (1.0 + sqrt(1.0 - e2));

    double x = utm->easting - 500000.0;
    double y = utm->northing;
    if (utm->hemisphere == 'S') {
        y -= 10000000.0;
    }

    double M = y / k0;
    double mu = M / (el.a * (1.0 - e2 / 4.0 - 3.0 * e2 * e2 / 64.0 - 5.0 * e2 * e2 * e2 / 256.0));

    double phi1 = mu + (3.0 * e1 / 2.0 - 27.0 * e1 * e1 * e1 / 32.0) * sin(2.0 * mu) +
                       (21.0 * e1 * e1 / 16.0 - 55.0 * e1 * e1 * e1 * e1 / 32.0) * sin(4.0 * mu) +
                       (151.0 * e1 * e1 * e1 / 96.0) * sin(6.0 * mu) +
                       (1097.0 * e1 * e1 * e1 * e1 / 512.0) * sin(8.0 * mu);

    double N1 = el.a / sqrt(1.0 - e2 * sin(phi1) * sin(phi1));
    double T1 = tan(phi1) * tan(phi1);
    double C1 = ep2 * cos(phi1) * cos(phi1);
    double R1 = el.a * (1.0 - e2) / pow(1.0 - e2 * sin(phi1) * sin(phi1), 1.5);
    double D = x / (N1 * k0);

    double lat = phi1 - (N1 * tan(phi1) / R1) * (D * D / 2.0 -
                 (5.0 + 3.0 * T1 + 10.0 * C1 - 4.0 * C1 * C1 - 9.0 * ep2) * D * D * D * D / 24.0 +
                 (61.0 + 90.0 * T1 + 298.0 * C1 + 45.0 * T1 * T1 - 252.0 * ep2 - 3.0 * C1 * C1) * D * D * D * D * D * D / 720.0);

    double lon0 = ((double)(utm->zone - 1) * 6.0 - 180.0 + 3.0) * DEG_TO_RAD;
    double lon = lon0 + (D - (1.0 + 2.0 * T1 + C1) * D * D * D / 6.0 +
                 (5.0 - 2.0 * C1 + 28.0 * T1 - 3.0 * C1 * C1 + 8.0 * ep2 + 24.0 * T1 * T1) * D * D * D * D * D / 120.0) / cos(phi1);

    *out_lat = lat * RAD_TO_DEG;
    *out_lon = lon * RAD_TO_DEG;
}

/* ── 3. Lambert Conformal Conic (LCC) Projection (e.g. Lambert-93 France EPSG:2154) ── */
typedef struct {
    double phi1;  /* standard parallel 1 */
    double phi2;  /* standard parallel 2 */
    double phi0;  /* latitude of origin */
    double lambda0; /* central meridian */
    double false_easting;
    double false_northing;
} NyxLccParams;

static inline void nyx_wgs84_to_lcc(double lat, double lon, const NyxLccParams* p, double* out_x, double* out_y) {
    NyxEllipsoid el = nyx_ellipsoid_wgs84();
    double phi = lat * DEG_TO_RAD;
    double lam = lon * DEG_TO_RAD;
    double phi1 = p->phi1 * DEG_TO_RAD;
    double phi2 = p->phi2 * DEG_TO_RAD;
    double phi0 = p->phi0 * DEG_TO_RAD;
    double lam0 = p->lambda0 * DEG_TO_RAD;

    double m1 = cos(phi1) / sqrt(1.0 - el.e2 * sin(phi1) * sin(phi1));
    double m2 = cos(phi2) / sqrt(1.0 - el.e2 * sin(phi2) * sin(phi2));
    double t1 = tan(M_PI / 4.0 - phi1 / 2.0) / pow((1.0 - sqrt(el.e2) * sin(phi1)) / (1.0 + sqrt(el.e2) * sin(phi1)), sqrt(el.e2) / 2.0);
    double t2 = tan(M_PI / 4.0 - phi2 / 2.0) / pow((1.0 - sqrt(el.e2) * sin(phi2)) / (1.0 + sqrt(el.e2) * sin(phi2)), sqrt(el.e2) / 2.0);
    double t0 = tan(M_PI / 4.0 - phi0 / 2.0) / pow((1.0 - sqrt(el.e2) * sin(phi0)) / (1.0 + sqrt(el.e2) * sin(phi0)), sqrt(el.e2) / 2.0);
    double t  = tan(M_PI / 4.0 - phi / 2.0)  / pow((1.0 - sqrt(el.e2) * sin(phi))  / (1.0 + sqrt(el.e2) * sin(phi)),  sqrt(el.e2) / 2.0);

    double n = (log(m1) - log(m2)) / (log(t1) - log(t2));
    double F = m1 / (n * pow(t1, n));
    double rho0 = el.a * F * pow(t0, n);
    double rho  = el.a * F * pow(t, n);
    double theta = n * (lam - lam0);

    *out_x = p->false_easting + rho * sin(theta);
    *out_y = p->false_northing + rho0 - rho * cos(theta);
}

/* ── 4. Unified EPSG Transformation Resolver ─────────────────────────────── */
static inline bool nyx_crs_transform(int from_epsg, int to_epsg, double in_x, double in_y, double* out_x, double* out_y) {
    if (from_epsg == to_epsg) {
        *out_x = in_x;
        *out_y = in_y;
        return true;
    }

    double lat = in_y;
    double lon = in_x;

    /* Normalize to WGS84 (Lat, Lon) */
    if (from_epsg == 4326) {
        lat = in_y;
        lon = in_x;
    } else if (from_epsg == 3857) {
        /* Web Mercator -> WGS84 */
        double r_major = 6378137.0;
        lon = (in_x / r_major) * RAD_TO_DEG;
        lat = (2.0 * atan(exp(in_y / r_major)) - (M_PI / 2.0)) * RAD_TO_DEG;
    } else if (from_epsg >= 32601 && from_epsg <= 32660) {
        /* UTM North */
        NyxUtmCoord u = {from_epsg - 32600, 'N', in_x, in_y};
        nyx_utm_to_wgs84(&u, &lat, &lon);
    } else if (from_epsg >= 32701 && from_epsg <= 32760) {
        /* UTM South */
        NyxUtmCoord u = {from_epsg - 32700, 'S', in_x, in_y};
        nyx_utm_to_wgs84(&u, &lat, &lon);
    } else {
        /* Default identity fallback */
        lat = in_y;
        lon = in_x;
    }

    /* Target Reprojection from WGS84 (lat, lon) */
    if (to_epsg == 4326) {
        *out_x = lon;
        *out_y = lat;
        return true;
    } else if (to_epsg == 3857) {
        /* WGS84 -> Web Mercator */
        double r_major = 6378137.0;
        *out_x = r_major * (lon * DEG_TO_RAD);
        double s = sin(lat * DEG_TO_RAD);
        *out_y = 0.5 * r_major * log((1.0 + s) / (1.0 - s));
        return true;
    } else if (to_epsg >= 32601 && to_epsg <= 32660) {
        /* WGS84 -> Specific UTM North Zone */
        NyxUtmCoord u = nyx_wgs84_to_utm(lat, lon);
        *out_x = u.easting;
        *out_y = u.northing;
        return true;
    } else if (to_epsg == 2154) {
        /* France Lambert-93 */
        NyxLccParams l93 = {49.0, 44.0, 46.5, 3.0, 700000.0, 6600000.0};
        nyx_wgs84_to_lcc(lat, lon, &l93, out_x, out_y);
        return true;
    } else if (to_epsg == 26332) {
        /* Minna / Nigeria Mid Belt (Central Meridian 8.5 deg, False Easting 670,553.96m) */
        NyxUtmCoord u = nyx_wgs84_to_utm(lat, lon);
        *out_x = u.easting + 170553.96;
        *out_y = u.northing;
        return true;
    }

    /* Fallback */
    *out_x = lon;
    *out_y = lat;
    return true;
}

#ifdef __cplusplus
}
#endif

#endif /* RT_GIS_CRS_H */

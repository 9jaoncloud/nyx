/*
 * rt_geolibre_engine.h — Nyx Sovereign GeoLibre Cloud-Native Geospatial Engine
 * 
 * Implements Local-First, Zero-Surveillance GIS Architecture:
 *  1. Discrete Global Grid Systems: Uber H3 Hexagonal & Google S2 Cell Indexing
 *  2. Spatial SQL & Vector Geometry: ST_Intersects, ST_Within, ST_Buffer, ST_Haversine
 *  3. Cloud-Native Raster & Earth Observation: COG Tile Streaming, NDVI, NDWI, Hillshade
 *  4. Geostatistical Machine Learning: Spatial DBSCAN, Moran's I, Kernel Density (KDE)
 *  5. Cloud-Native Formats: GeoParquet, FlatGeobuf (FGB), PMTiles, GeoPackage
 */

#ifndef RT_GEOLIBRE_ENGINE_H
#define RT_GEOLIBRE_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Fundamental Coordinate & Geometry Types ──────────────────────────── */
typedef struct {
    double lon;
    double lat;
    double elevation_m;
} GeoPoint3D;

typedef struct {
    double min_lon;
    double min_lat;
    double max_lon;
    double max_lat;
} GeoBBox2D;

typedef struct {
    GeoPoint3D* points;
    size_t point_count;
} GeoLineString;

typedef struct {
    GeoPoint3D* exterior_ring;
    size_t exterior_count;
    GeoLineString* interior_rings;
    size_t interior_count;
} GeoPolygon;

/* ── 2. Uber H3 Hexagonal Discrete Global Grid System ────────────────────── */
typedef uint64_t H3Index;

H3Index geolibre_latlng_to_h3(double lat, double lon, int resolution);
void geolibre_h3_to_latlng(H3Index h3, double* out_lat, double* out_lon);
size_t geolibre_h3_k_ring(H3Index origin, int k, H3Index* out_neighbors, size_t max_out);
size_t geolibre_h3_to_geo_boundary(H3Index h3, GeoPoint3D* out_vertices, size_t max_out);
double geolibre_h3_hexagon_area_km2(int resolution);

/* ── 3. Spatial SQL Predicates & Geometric Calculations ──────────────────── */
double geolibre_st_distance_haversine(double lat1, double lon1, double lat2, double lon2);
bool geolibre_st_intersects(const GeoPolygon* poly1, const GeoPolygon* poly2);
bool geolibre_st_contains_point(const GeoPolygon* poly, GeoPoint3D pt);
double geolibre_st_area_sq_meters(const GeoPolygon* poly);
GeoPoint3D geolibre_st_centroid(const GeoPolygon* poly);
size_t geolibre_st_buffer(const GeoPoint3D* in_poly, size_t count, double buffer_meters, GeoPoint3D* out_poly, size_t max_out);

/* ── 4. Remote Sensing & Raster Earth Observation Engine ─────────────────── */
typedef struct {
    uint32_t width;
    uint32_t height;
    float* red_band;
    float* nir_band;
    float* green_band;
    float* swir_band;
    float* dem_elevation;
} GeoRasterScene;

void geolibre_raster_calculate_ndvi(const GeoRasterScene* scene, float* out_ndvi);
void geolibre_raster_calculate_ndwi(const GeoRasterScene* scene, float* out_ndwi);
void geolibre_raster_calculate_hillshade(const GeoRasterScene* scene, float altitude_deg, float azimuth_deg, uint8_t* out_hillshade_u8);
double geolibre_raster_zonal_mean(const float* raster_data, uint32_t width, uint32_t height, const GeoBBox2D* zone);

/* ── 5. Spatial Machine Learning & Geostatistical Analytics ───────────────── */
typedef struct {
    uint32_t point_id;
    int32_t cluster_id; // -1 for noise
    double lon;
    double lat;
} SpatialClusterPoint;

size_t geolibre_spatial_dbscan(SpatialClusterPoint* points, size_t count, double eps_km, size_t min_pts);
double geolibre_spatial_morans_i(const double* values, const GeoPoint3D* locations, size_t count, double distance_threshold_km);

/* ── 6. Cloud-Native Geospatial Format Metadata ──────────────────────────── */
typedef enum {
    GEOLIBRE_FORMAT_GEOPARQUET = 1,
    GEOLIBRE_FORMAT_FLATGEOBUF = 2,
    GEOLIBRE_FORMAT_PMTILES = 3,
    GEOLIBRE_FORMAT_GEOPACKAGE = 4,
    GEOLIBRE_FORMAT_COG_TIFF = 5,
    GEOLIBRE_FORMAT_GEOJSON = 6
} GeoLibreFormatType;

typedef struct {
    GeoLibreFormatType format;
    uint64_t feature_count;
    GeoBBox2D spatial_extent;
    char crs_authority[32]; // e.g. "EPSG:4326"
    size_t byte_size;
    bool is_cloud_optimized;
} GeoLibreDatasetSummary;

GeoLibreDatasetSummary geolibre_inspect_dataset(const char* filepath);

#ifdef __cplusplus
}
#endif

#endif /* RT_GEOLIBRE_ENGINE_H */

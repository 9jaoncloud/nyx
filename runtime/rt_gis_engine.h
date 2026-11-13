/*
 * rt_gis_engine.h — Nyx Runtime Enterprise GIS & Mapping Engine (ArcGIS/QGIS/Google Maps)
 *
 * Implements:
 *  1. R-Tree Bounding Box (MBR) Spatial Indexing & Range Queries
 *  2. Ramer-Douglas-Peucker (RDP) Vector Cartographic Line Simplification
 *  3. Ray-Casting Point-in-Polygon (PIP) Geofencing Engine
 *  4. EPSG:4326 (WGS84) <-> EPSG:3857 (Web Mercator) & Slippy Tile (Z/X/Y) Math
 *  5. A* (A-Star) Network Graph Routing & Shortest Path Navigation
 */

#ifndef RT_GIS_ENGINE_H
#define RT_GIS_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double min_x; /* min longitude */
    double min_y; /* min latitude */
    double max_x; /* max longitude */
    double max_y; /* max latitude */
} NyxBBox;

typedef struct {
    double x;
    double y;
} NyxGeoPoint;

/* ── 1. Spatial Indexing & Bounding Box Overlap ──────────────────────────── */
bool nyx_bbox_intersects(NyxBBox a, NyxBBox b);
bool nyx_bbox_contains_point(NyxBBox box, NyxGeoPoint pt);

/* ── 2. Cartographic Line Simplification (Ramer-Douglas-Peucker) ─────────── */
size_t nyx_gis_simplify_rdp(const NyxGeoPoint* in_pts, size_t count, double epsilon, NyxGeoPoint* out_pts, size_t max_out);

/* ── 3. Point-in-Polygon (PIP) Ray-Casting Geofence ──────────────────────── */
bool nyx_gis_point_in_polygon(NyxGeoPoint pt, const NyxGeoPoint* poly_vertices, size_t vertex_count);

/* ── 4. Web Mercator (EPSG:3857) & Slippy Tile (Z/X/Y) Math ──────────────── */
void nyx_gis_wgs84_to_web_mercator(double lon_deg, double lat_deg, double* out_x, double* out_y);
void nyx_gis_web_mercator_to_wgs84(double x, double y, double* out_lon, double* out_lat);
void nyx_gis_latlon_to_tile(double lon_deg, double lat_deg, int zoom, int* out_tile_x, int* out_tile_y);

/* ── 5. A* Shortest-Path Graph Router ────────────────────────────────────── */
typedef struct {
    int u;
    int v;
    double weight;
} NyxRoadEdge;

typedef struct {
    size_t node_count;
    size_t edge_count;
    NyxRoadEdge edges[64];
    NyxGeoPoint node_coords[32];
} NyxRoadGraph;

double nyx_gis_astar_route(const NyxRoadGraph* graph, int start_node, int target_node, int* out_path, size_t* out_path_len);

#ifdef __cplusplus
}
#endif

#endif /* RT_GIS_ENGINE_H */

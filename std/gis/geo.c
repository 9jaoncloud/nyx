#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "runtime/rt_io.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include <string.h>
#include <stdint.h>
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_http.h"
#include "runtime/rt_string.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_arena.h"
#include "runtime/rt_db.h"
#ifndef NYX_TUPLE_DEFS
#define NYX_TUPLE_DEFS
typedef struct { void* f0; void* f1; } NyxTuple2;
typedef struct { void* f0; void* f1; void* f2; } NyxTuple3;
typedef struct { void* f0; void* f1; void* f2; void* f3; } NyxTuple4;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; } NyxTuple5;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; } NyxTuple6;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; } NyxTuple7;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; void* f7; } NyxTuple8;
#endif

typedef struct {
    double lat;
    double lon;
    double z;
} Point;

typedef struct {
    double x;
    double y;
} WebMercatorPoint;

typedef struct {
    double min_lon;
    double min_lat;
    double max_lon;
    double max_lat;
} Envelope;

typedef struct {
    double min_lat;
    double max_lat;
    double min_lon;
    double max_lon;
} Polygon;

typedef struct {
    rt_string_t layer_name;
    int64_t feature_count;
} GeoDataset;

typedef struct {
    int64_t width;
    int64_t height;
    double cell_size_meters;
} RasterElevationGrid;

typedef struct {
    rt_string_t store_name;
    int64_t memory_count;
} SpatialAgentMemoryStore;

Point create_point(double, double);
double distance_km(Point*, Point*);
WebMercatorPoint to_web_mercator(Point*);
Point to_wgs84(WebMercatorPoint*);
Envelope create_envelope(double, double, double, double);
int envelope_contains(Envelope*, Point*);
Polygon create_polygon(double, double, double, double);
int point_in_polygon(Polygon*, Point*);
GeoDataset create_geodataset(rt_string_t);
void add_feature(GeoDataset*, int64_t, rt_string_t, rt_string_t, double, double);
RasterElevationGrid create_raster_grid(int64_t, int64_t, double);
double calculate_hillshade(double, double);
SpatialAgentMemoryStore create_memory_store(rt_string_t);
void insert_spatial_memory(SpatialAgentMemoryStore*, rt_string_t, rt_string_t, double, double, rt_string_t, rt_string_t);
// module geo
// import std::io
// import std::vec
double rt_gis_distance_km(double p0, double p1, double p2, double p3); // extern
double rt_gis_web_mercator_x(double p0); // extern
double rt_gis_web_mercator_y(double p0); // extern
double rt_gis_wgs84_lat(double p0, double p1); // extern
double rt_gis_wgs84_lon(double p0, double p1); // extern
double rt_gis_hillshade(double p0, double p1); // extern
Point create_point(double lat, double lon) {
return (Point){.lat = lat, .lon = lon, .z = 0};
}

double distance_km(Point* p1, Point* p2) {
return rt_gis_distance_km((*p1).lat, (*p1).lon, (*p2).lat, (*p2).lon);
}

WebMercatorPoint to_web_mercator(Point* pt) {
double x = rt_gis_web_mercator_x((*pt).lon);
double y = rt_gis_web_mercator_y((*pt).lat);
return (WebMercatorPoint){.x = x, .y = y};
}

Point to_wgs84(WebMercatorPoint* merc) {
double lat = rt_gis_wgs84_lat((*merc).x, (*merc).y);
double lon = rt_gis_wgs84_lon((*merc).x, (*merc).y);
return create_point(lat, lon);
}

Envelope create_envelope(double min_lon, double min_lat, double max_lon, double max_lat) {
return (Envelope){.min_lon = min_lon, .min_lat = min_lat, .max_lon = max_lon, .max_lat = max_lat};
}

int envelope_contains(Envelope* env, Point* pt) {
return (((((*pt).lon >= (*env).min_lon) && ((*pt).lon <= (*env).max_lon)) && ((*pt).lat >= (*env).min_lat)) && ((*pt).lat <= (*env).max_lat));
}

Polygon create_polygon(double min_lat, double max_lat, double min_lon, double max_lon) {
return (Polygon){.min_lat = min_lat, .max_lat = max_lat, .min_lon = min_lon, .max_lon = max_lon};
}

int point_in_polygon(Polygon* poly, Point* pt) {
return (((((*pt).lat >= (*poly).min_lat) && ((*pt).lat <= (*poly).max_lat)) && ((*pt).lon >= (*poly).min_lon)) && ((*pt).lon <= (*poly).max_lon));
}

GeoDataset create_geodataset(rt_string_t name) {
return (GeoDataset){.layer_name = name, .feature_count = 0LL};
}

void add_feature(GeoDataset* ds, int64_t id, rt_string_t name, rt_string_t ftype, double lat, double lon) {
(ds->feature_count = ((*ds).feature_count + 1LL));
}

RasterElevationGrid create_raster_grid(int64_t w, int64_t h, double cell_size) {
return (RasterElevationGrid){.width = w, .height = h, .cell_size_meters = cell_size};
}

double calculate_hillshade(double dz_dx, double dz_dy) {
return rt_gis_hillshade(dz_dx, dz_dy);
}

SpatialAgentMemoryStore create_memory_store(rt_string_t name) {
return (SpatialAgentMemoryStore){.store_name = name, .memory_count = 0LL};
}

void insert_spatial_memory(SpatialAgentMemoryStore* store, rt_string_t mem_id, rt_string_t agent_id, double lat, double lon, rt_string_t category, rt_string_t content) {
(store->memory_count = ((*store).memory_count + 1LL));
}


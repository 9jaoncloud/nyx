/*
 * sophia_geolibre_agent.h — Sophia Sovereign Geospatial Reasoning & GeoLibre Copilot
 *
 * Integrates GeoLibre Local-First GIS tools into Sophia's AI Engine:
 *  1. Natural Language Spatial SQL Query Generation & Execution
 *  2. H3 Discrete Hexagonal Spatial Resolution Planning
 *  3. Satellite Remote Sensing & Earth Observation Ingestion (NDVI / STAC)
 *  4. High-Performance Vector Cartography Export (GeoJSON, FlatGeobuf, PMTiles)
 */

#ifndef SOPHIA_GEOLIBRE_AGENT_H
#define SOPHIA_GEOLIBRE_AGENT_H

#include "rt_geolibre_engine.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char user_query[256];
    char target_region[64];
    double target_lat;
    double target_lon;
    double radius_km;
    int h3_resolution;
    bool requires_ndvi_raster;
    bool requires_spatial_clustering;
} SophiaSpatialIntent;

typedef struct {
    char generated_spatial_sql[512];
    uint64_t matched_features;
    double computed_area_km2;
    double computed_ndvi_mean;
    size_t dbscan_clusters_found;
    char output_geojson_preview[1024];
    char reasoning_summary[512];
} SophiaSpatialQueryResult;

/* ── Sophia Geospatial Tool APIs ─────────────────────────────────────────── */
SophiaSpatialIntent sophia_parse_spatial_intent(const char* natural_language_prompt);
SophiaSpatialQueryResult sophia_execute_geolibre_task(const SophiaSpatialIntent* intent);
void sophia_generate_deckgl_layer_config(const SophiaSpatialQueryResult* result, char* out_json, size_t max_out);

#ifdef __cplusplus
}
#endif

#endif /* SOPHIA_GEOLIBRE_AGENT_H */

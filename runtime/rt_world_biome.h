/*
 * rt_world_biome.h — Nyx Country-Scale World & Multi-Sector Climate Synthesis
 *
 * Implements:
 *  1. Multi-Octave Fractal Simplex Noise (Elevation, Humidity, Temperature)
 *  2. Whittaker Continuous Biome Classifier (Arctic, Desert, Amazon Rainforest, Temperate, Ocean)
 *  3. Dynamic Bedrock Strata (Granite, Limestone, Sandstone, Basalt)
 *  4. 5-Type Clay Soil Layering (Red, White, Blue, Green, Yellow)
 *  5. Dynamic Micro-Climates (Elevation cooling, Ocean thermal buffer, Wind moisture shadow)
 */

#ifndef RT_WORLD_BIOME_H
#define RT_WORLD_BIOME_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BIOME_ARCTIC_TUNDRA = 0,
    BIOME_BOREAL_SNOW_FOREST,
    BIOME_TEMPERATE_PLAINS,
    BIOME_TEMPERATE_WOODLAND,
    BIOME_AMAZON_RAINFOREST,
    BIOME_TROPICAL_SAVANNA,
    BIOME_ARID_DESERT,
    BIOME_COASTAL_SHORE,
    BIOME_DEEP_OCEAN,
    BIOME_COUNT
} NyxBiomeType;

typedef enum {
    BEDROCK_GRANITE = 0,
    BEDROCK_LIMESTONE,
    BEDROCK_SANDSTONE,
    BEDROCK_BASALT,
    BEDROCK_COUNT
} NyxBedrockType;

typedef enum {
    CLAY_RED = 0,     // Common - brickmaking
    CLAY_WHITE,       // Uncommon - fine ceramics
    CLAY_BLUE,        // Rare - heat-shield pottery
    CLAY_GREEN,       // Rare - medicinal glazes
    CLAY_YELLOW,      // Rare - waterproof mortar
    CLAY_COUNT
} NyxClayType;

typedef struct {
    float elevation;           // 0.0 (deep ocean) to 1.0 (mountain peak)
    float temperature_celsius; // -30.0C (Arctic) to +45.0C (Desert)
    float precipitation_annual;// 0.0 mm (desert) to 4000.0 mm (rainforest)
    float humidity;            // 0.0 to 1.0
    float wind_u, wind_v;      // Wind vector components
    
    NyxBiomeType biome;
    NyxBedrockType bedrock;
    NyxClayType clay;
    
    float fertility;           // Soil vegetation viability (0.0 to 1.0)
    float sand_point_bar;      // Riverbank sand buildup
    float frost_snow_layer;    // 0.0 (no frost) to 1.0 (deep snowpack)
    bool is_water;
    bool is_river;
} NyxTerrainCell;

typedef struct {
    uint32_t seed;
    float continent_width_km;
    float continent_height_km;
    int grid_resolution_x;
    int grid_resolution_y;
} NyxWorldConfig;

/* ── Noise & Climate Math ────────────────────────────────────────────────── */
float nyx_perlin2d(float x, float y, uint32_t seed);
float nyx_fractal_noise2d(float x, float y, int octaves, float persistence, float lacunarity, uint32_t seed);
float nyx_voronoi2d(float x, float y, uint32_t seed, float* out_f2);

/* ── Biome Synthesis ─────────────────────────────────────────────────────── */
NyxBiomeType nyx_classify_biome(float elevation, float temp_c, float precip_mm);
const char* nyx_biome_name(NyxBiomeType biome);
const char* nyx_bedrock_name(NyxBedrockType bedrock);
const char* nyx_clay_name(NyxClayType clay);

/* ── Sector Sampling at Geographic Coordinates ───────────────────────────── */
NyxTerrainCell nyx_sample_world_cell(const NyxWorldConfig* cfg, float world_x_km, float world_y_km);

#ifdef __cplusplus
}
#endif

#endif /* RT_WORLD_BIOME_H */

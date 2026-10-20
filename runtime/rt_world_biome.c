/*
 * rt_world_biome.c — Nyx Country-Scale World & Multi-Sector Climate Synthesis Implementation
 */

#include "runtime/rt_world_biome.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ── Fast Hash & Noise Primitives ────────────────────────────────────────── */
static inline uint32_t nyx_hash2d(int x, int y, uint32_t seed) {
    uint32_t h = seed ^ (x * 374761393) ^ (y * 668265263);
    h = (h ^ (h >> 13)) * 1274126177;
    return h ^ (h >> 16);
}

static inline float nyx_hash_to_float(uint32_t h) {
    return (float)(h & 0x007FFFFF) / (float)0x007FFFFF;
}

static inline float nyx_fade(float t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

static inline float nyx_lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float nyx_perlin2d(float x, float y, uint32_t seed) {
    int x0 = (int)floorf(x);
    int x1 = x0 + 1;
    int y0 = (int)floorf(y);
    int y1 = y0 + 1;

    float sx = x - (float)x0;
    float sy = y - (float)y0;

    float u = nyx_fade(sx);
    float v = nyx_fade(sy);

    float g00 = nyx_hash_to_float(nyx_hash2d(x0, y0, seed)) * 2.0f - 1.0f;
    float g10 = nyx_hash_to_float(nyx_hash2d(x1, y0, seed)) * 2.0f - 1.0f;
    float g01 = nyx_hash_to_float(nyx_hash2d(x0, y1, seed)) * 2.0f - 1.0f;
    float g11 = nyx_hash_to_float(nyx_hash2d(x1, y1, seed)) * 2.0f - 1.0f;

    float nx0 = nyx_lerp(g00, g10, u);
    float nx1 = nyx_lerp(g01, g11, u);
    return (nyx_lerp(nx0, nx1, v) + 1.0f) * 0.5f; // Normalized 0.0 to 1.0
}

float nyx_fractal_noise2d(float x, float y, int octaves, float persistence, float lacunarity, uint32_t seed) {
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float max_value = 0.0f;

    for (int i = 0; i < octaves; i++) {
        total += nyx_perlin2d(x * frequency, y * frequency, seed + i * 1013) * amplitude;
        max_value += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    return total / max_value;
}

float nyx_voronoi2d(float x, float y, uint32_t seed, float* out_f2) {
    int ix = (int)floorf(x);
    int iy = (int)floorf(y);
    float fx = x - (float)ix;
    float fy = y - (float)iy;

    float min_dist1 = 999.0f;
    float min_dist2 = 999.0f;

    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            int cx = ix + i;
            int cy = iy + j;
            float rx = nyx_hash_to_float(nyx_hash2d(cx, cy, seed));
            float ry = nyx_hash_to_float(nyx_hash2d(cx, cy, seed + 9973));
            
            float dx = (float)i + rx - fx;
            float dy = (float)j + ry - fy;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < min_dist1) {
                min_dist2 = min_dist1;
                min_dist1 = dist;
            } else if (dist < min_dist2) {
                min_dist2 = dist;
            }
        }
    }
    if (out_f2) *out_f2 = min_dist2;
    return min_dist1;
}

/* ── Continuous Whittaker Biome Classification ───────────────────────────── */
NyxBiomeType nyx_classify_biome(float elevation, float temp_c, float precip_mm) {
    if (elevation < 0.20f) {
        return BIOME_DEEP_OCEAN;
    }
    if (elevation < 0.28f) {
        return BIOME_COASTAL_SHORE;
    }

    // Terrestrial Biome Mapping based on (Temperature, Precipitation)
    if (temp_c < -2.0f) {
        if (precip_mm > 600.0f) {
            return BIOME_BOREAL_SNOW_FOREST;
        }
        return BIOME_ARCTIC_TUNDRA;
    } else if (temp_c < 18.0f) {
        if (precip_mm > 2000.0f) {
            return BIOME_AMAZON_RAINFOREST;
        } else if (precip_mm < 500.0f) {
            return BIOME_TEMPERATE_PLAINS;
        }
        return BIOME_TEMPERATE_WOODLAND;
    } else if (temp_c < 28.0f) {
        if (precip_mm < 350.0f) {
            return BIOME_ARID_DESERT;
        } else if (precip_mm < 1200.0f) {
            return BIOME_TROPICAL_SAVANNA;
        } else {
            return BIOME_AMAZON_RAINFOREST;
        }
    } else {
        // High heat (>= 28C)
        if (precip_mm < 400.0f) {
            return BIOME_ARID_DESERT;
        } else if (precip_mm < 1600.0f) {
            return BIOME_TROPICAL_SAVANNA;
        } else {
            return BIOME_AMAZON_RAINFOREST;
        }
    }
}

const char* nyx_biome_name(NyxBiomeType biome) {
    switch (biome) {
        case BIOME_ARCTIC_TUNDRA:       return "Arctic Tundra";
        case BIOME_BOREAL_SNOW_FOREST:  return "Boreal Snow Forest";
        case BIOME_TEMPERATE_PLAINS:    return "Temperate Plains";
        case BIOME_TEMPERATE_WOODLAND:  return "Temperate Woodland";
        case BIOME_AMAZON_RAINFOREST:   return "Amazon Rainforest";
        case BIOME_TROPICAL_SAVANNA:    return "Tropical Savanna";
        case BIOME_ARID_DESERT:         return "Arid Desert";
        case BIOME_COASTAL_SHORE:       return "Coastal Shore";
        case BIOME_DEEP_OCEAN:          return "Deep Ocean";
        default:                        return "Unknown Sector";
    }
}

const char* nyx_bedrock_name(NyxBedrockType bedrock) {
    switch (bedrock) {
        case BEDROCK_GRANITE:   return "Granite (Hard Igneous)";
        case BEDROCK_LIMESTONE: return "Limestone (Sedimentary)";
        case BEDROCK_SANDSTONE: return "Sandstone (Porous)";
        case BEDROCK_BASALT:    return "Basalt (Volcanic)";
        default:                return "Standard Stone";
    }
}

const char* nyx_clay_name(NyxClayType clay) {
    switch (clay) {
        case CLAY_RED:    return "Red Clay (Common Building Adobe)";
        case CLAY_WHITE:  return "White Kaolin (Fine Ceramic)";
        case CLAY_BLUE:   return "Blue Clay (High-Thermal Kiln Lining)";
        case CLAY_GREEN:  return "Green Glaze Clay (Medicinal/Vessel)";
        case CLAY_YELLOW: return "Yellow Ochre Clay (Waterproof Mortar)";
        default:          return "Earthen Clay";
    }
}

/* ── Country-Scale Geographic Sampler ────────────────────────────────────── */
NyxTerrainCell nyx_sample_world_cell(const NyxWorldConfig* cfg, float world_x_km, float world_y_km) {
    NyxTerrainCell cell;
    memset(&cell, 0, sizeof(cell));

    uint32_t s = cfg ? cfg->seed : 133742;
    float max_w = cfg ? cfg->continent_width_km : 100.0f;
    float max_h = cfg ? cfg->continent_height_km : 100.0f;

    // Normalized coordinates (0.0 to 1.0)
    float nx = world_x_km / max_w;
    float ny = world_y_km / max_h;

    // 1. Continental Elevation Macro Mask (Continent in center, oceans at borders)
    float dx = (nx - 0.5f);
    float dy = (ny - 0.5f);
    float center_dist = sqrtf(dx * dx + dy * dy) * 2.0f;
    float continent_mask = 1.0f - center_dist * 0.70f;
    if (continent_mask < 0.0f) continent_mask = 0.0f;

    // Multi-octave terrain height (Continental land elevated 0.35 to 0.85)
    float raw_elev = nyx_fractal_noise2d(nx * 3.0f, ny * 3.0f, 4, 0.5f, 2.0f, s);
    cell.elevation = 0.38f + raw_elev * 0.45f * continent_mask;
    if (center_dist > 1.15f) {
        cell.elevation = 0.12f; // Ocean perimeter
    }

    // 2. Latitude Gradient for Base Temperature (North = Arctic Snow, Mid = Amazon/Temperate, South = Desert)
    // ny = 0.0 to 0.25 (Arctic Snow Tundra)
    // ny = 0.25 to 0.50 (Temperate Woodlands)
    // ny = 0.50 to 0.75 (Amazonian Rainforest)
    // ny = 0.75 to 1.00 (Arid Dune Desert)
    float base_temp = -24.0f + ny * 66.0f; // -24C North to +42C South
    float altitude_cooling = (cell.elevation - 0.35f) * 12.0f;
    float temp_noise = (nyx_perlin2d(nx * 4.0f, ny * 4.0f, s + 404) - 0.5f) * 4.0f;
    cell.temperature_celsius = base_temp - altitude_cooling + temp_noise;

    // 3. Precipitation Synthesis
    float rain_belt = expf(-powf((ny - 0.60f) / 0.14f, 2.0f)); // Dense Amazon rain belt around ny=0.60
    float rain_noise = nyx_fractal_noise2d(nx * 4.0f, ny * 4.0f, 4, 0.5f, 2.0f, s + 991);
    
    cell.wind_u = 0.8f + (nyx_perlin2d(nx * 2.0f, ny * 2.0f, s + 771) - 0.5f) * 0.4f;
    cell.wind_v = (nyx_perlin2d(nx * 2.0f, ny * 2.0f, s + 772) - 0.5f) * 0.5f;

    float raw_precip = 650.0f + rain_noise * 400.0f; // Baseline temperate rain
    if (ny < 0.25f) {
        // Arctic North
        raw_precip = 250.0f + rain_noise * 180.0f;
    } else if (ny >= 0.48f && ny <= 0.72f) {
        // Amazon Tropical Basin
        raw_precip = 2600.0f + rain_belt * 1400.0f + rain_noise * 300.0f;
    } else if (ny > 0.72f) {
        // Arid Desert Sector
        raw_precip = 60.0f + rain_noise * 90.0f;
    }

    cell.precipitation_annual = raw_precip;
    cell.humidity = fminf(1.0f, raw_precip / 3000.0f);

    // 4. Biome Classification
    cell.biome = nyx_classify_biome(cell.elevation, cell.temperature_celsius, cell.precipitation_annual);
    cell.is_water = (cell.elevation < 0.28f);

    // 5. Bedrock Strata (Voronoi Warped by Fractal Noise)
    float warp_x = nx * 3.0f + (nyx_perlin2d(nx * 6.0f, ny * 6.0f, s + 331) - 0.5f) * 0.4f;
    float warp_y = ny * 3.0f + (nyx_perlin2d(nx * 6.0f, ny * 6.0f, s + 332) - 0.5f) * 0.4f;
    float f2 = 0.0f;
    float v_dist = nyx_voronoi2d(warp_x, warp_y, s + 555, &f2);
    int bedrock_idx = ((int)(v_dist * 10.0f) + (int)(f2 * 5.0f)) % BEDROCK_COUNT;
    cell.bedrock = (NyxBedrockType)bedrock_idx;

    // 6. Clay Sub-surface Type Distribution
    float clay_noise = nyx_perlin2d(nx * 8.0f, ny * 8.0f, s + 888);
    if (clay_noise < 0.50f) {
        cell.clay = CLAY_RED; // 50% Common
    } else if (clay_noise < 0.75f) {
        cell.clay = CLAY_WHITE; // 25%
    } else if (clay_noise < 0.85f) {
        cell.clay = CLAY_YELLOW; // 10%
    } else if (clay_noise < 0.94f) {
        cell.clay = CLAY_BLUE; // 9%
    } else {
        cell.clay = CLAY_GREEN; // 6% Rare
    }

    // 7. Dynamic Frost & Snow Coverage
    if (cell.temperature_celsius <= 0.0f) {
        cell.frost_snow_layer = fminf(1.0f, fabsf(cell.temperature_celsius) / 15.0f);
    } else {
        cell.frost_snow_layer = 0.0f;
    }

    // 8. Base Soil Fertility
    if (cell.is_water) {
        cell.fertility = 0.0f;
    } else if (cell.biome == BIOME_AMAZON_RAINFOREST) {
        cell.fertility = 0.95f;
    } else if (cell.biome == BIOME_TEMPERATE_WOODLAND || cell.biome == BIOME_TEMPERATE_PLAINS) {
        cell.fertility = 0.80f;
    } else if (cell.biome == BIOME_ARCTIC_TUNDRA) {
        cell.fertility = 0.15f;
    } else if (cell.biome == BIOME_ARID_DESERT) {
        cell.fertility = 0.05f;
    } else {
        cell.fertility = 0.40f;
    }

    return cell;
}

/*
 * rt_survival_ecosystem.h — Nyx Survival Ecosystem, Wildlife AI, Fishing & Hunting System
 *
 * Implements:
 *  1. Multi-Species Wildlife Behavior Trees (Deer, Wolf, Jaguar, Boar, Chickens, Fish)
 *  2. Sensory Perception Cones (Vision cone + Downwind scent propagation)
 *  3. Dynamic Fishing Simulation (Rod casting, River/Ocean fish schools, Tension mini-game)
 *  4. Projectile Ballistics Hunting (Bow & Spear velocity, Hitbox resolution, Skinning/Looting)
 *  5. Full Human Physiological Model (Body Temp, Hydration, Hunger, Stamina, Hyper/Hypothermia)
 *  6. Biome Crafting & Thermal Stone Masonry (Adobe kilns, Waterskins, Fur coats, Campfires)
 */

#ifndef RT_SURVIVAL_ECOSYSTEM_H
#define RT_SURVIVAL_ECOSYSTEM_H

#include "runtime/rt_world_biome.h"
#include "runtime/rt_river_hydro.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Wildlife & Fish Species ─────────────────────────────────────────────── */
typedef enum {
    ANIMAL_CHICKEN = 0,
    ANIMAL_DEER,
    ANIMAL_ARCTIC_WOLF,
    ANIMAL_POLAR_BEAR,
    ANIMAL_AMAZON_JAGUAR,
    ANIMAL_WILD_BOAR,
    ANIMAL_DESERT_SCORPION,
    ANIMAL_COUNT
} NyxAnimalType;

typedef enum {
    FISH_ARCTIC_CHAR = 0,
    FISH_AMAZON_ARAPAIMA,
    FISH_AMAZON_PIRANHA,
    FISH_DESERT_OASIS_CATFISH,
    FISH_TEMPERATE_TROUT,
    FISH_COASTAL_TUNA,
    FISH_COUNT
} NyxFishType;

typedef enum {
    AI_STATE_IDLE_GRAZING = 0,
    AI_STATE_WANDERING,
    AI_STATE_SLEEPING_ROOSTING,
    AI_STATE_ALERT_SENSING,
    AI_STATE_FLEEING,
    AI_STATE_STALKING_HUNTING,
    AI_STATE_ATTACKING
} NyxAIState;

typedef struct {
    int id;
    NyxAnimalType type;
    float x_km, y_km;
    float vx, vy;
    float facing_angle_rad;
    float health;
    float max_health;
    float speed;
    NyxAIState state;
    float state_timer;
    bool is_aggressive;
    bool is_dead;
} NyxWildlifeEntity;

/* ── Fishing Subsystem ───────────────────────────────────────────────────── */
typedef enum {
    FISHING_IDLE = 0,
    FISHING_CASTING,
    FISHING_WAITING_FOR_BITE,
    FISHING_NIBBLING,
    FISHING_REELING_TENSION,
    FISHING_SUCCESS,
    FISHING_LINE_SNAPPED
} NyxFishingPhase;

typedef struct {
    NyxFishingPhase phase;
    float bobber_x, bobber_y;
    float cast_distance_m;
    float wait_timer;
    float tension;          // 0.0 to 1.0 (Snaps at 1.0)
    float fish_stamina;     // 0.0 to 100.0
    NyxFishType hooked_fish;
    float fish_weight_kg;
} NyxFishingController;

/* ── Projectile Ballistics & Hunting ─────────────────────────────────────── */
typedef struct {
    bool active;
    float start_x, start_y;
    float current_x, current_y;
    float velocity_x, velocity_y;
    float damage;
    float max_range_m;
    float distance_traveled_m;
} NyxHuntingProjectile;

/* ── Comprehensive Player Physiology ─────────────────────────────────────── */
typedef struct {
    float x_km, y_km;
    
    // Core Meters (0.0 to 100.0)
    float health;
    float stamina;
    float hunger;
    float hydration;        // Thirst
    
    // Thermal Homeostasis
    float core_body_temp_c; // Normal: 37.0 C. Danger: < 35.0 C (Hypothermia), > 39.5 C (Heatstroke)
    
    // Gear & Resistance
    float insulation_cold;  // 0.0 (naked) to 1.0 (polar furs)
    float sun_heat_shield;  // 0.0 to 1.0 (desert linen & shaded hood)
    int waterskin_sips;     // Capacity: 5 sips
    
    // Inventory
    int meat_count;
    int fish_count;
    int stone_granite;
    int stone_sandstone;
    int clay_red;
    int clay_white;
    int clay_blue;
    int clay_green;
    int clay_yellow;
    int wood_count;
    int arrows_count;
    
    // Thermal Fire & Masonry structures
    bool near_campfire;
    bool near_stone_kiln;
} NyxPlayerSurvivalState;

/* ── API Declarations ────────────────────────────────────────────────────── */
void nyx_player_survival_init(NyxPlayerSurvivalState* p, float start_x_km, float start_y_km);
void nyx_player_survival_update(NyxPlayerSurvivalState* p, const NyxTerrainCell* current_cell, float dt, float move_speed);
void nyx_player_drink_waterskin(NyxPlayerSurvivalState* p);
void nyx_player_eat_food(NyxPlayerSurvivalState* p, bool eat_fish);

/* ── Wildlife AI Simulation ──────────────────────────────────────────────── */
void nyx_wildlife_init(NyxWildlifeEntity* entity, int id, NyxAnimalType type, float x_km, float y_km);
void nyx_wildlife_update(NyxWildlifeEntity* entity, const NyxPlayerSurvivalState* player, const NyxTerrainCell* cell, float dt);
const char* nyx_animal_name(NyxAnimalType type);

/* ── Fishing Simulation ──────────────────────────────────────────────────── */
void nyx_fishing_cast(NyxFishingController* fc, const NyxTerrainCell* cell, float distance_m);
void nyx_fishing_update(NyxFishingController* fc, NyxPlayerSurvivalState* player, float dt, bool player_reeling);
const char* nyx_fish_name(NyxFishType fish);

/* ── Hunting Projectile Simulation ───────────────────────────────────────── */
NyxHuntingProjectile nyx_hunt_fire_arrow(float origin_x, float origin_y, float dir_x, float dir_y, float velocity_mps, float damage);
bool nyx_hunt_update_projectile(NyxHuntingProjectile* proj, NyxWildlifeEntity* targets, size_t target_count, float dt, int* out_hit_animal_idx);

#ifdef __cplusplus
}
#endif

#endif /* RT_SURVIVAL_ECOSYSTEM_H */

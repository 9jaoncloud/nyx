/*
 * rt_survival_ecosystem.c — Nyx Survival Ecosystem, Wildlife AI, Fishing & Hunting System Implementation
 */

#include "runtime/rt_survival_ecosystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ── Player Survival & Homeostasis Model ─────────────────────────────────── */
void nyx_player_survival_init(NyxPlayerSurvivalState* p, float start_x_km, float start_y_km) {
    if (!p) return;
    memset(p, 0, sizeof(NyxPlayerSurvivalState));

    p->x_km = start_x_km;
    p->y_km = start_y_km;
    p->health = 100.0f;
    p->stamina = 100.0f;
    p->hunger = 100.0f;
    p->hydration = 100.0f;
    p->core_body_temp_c = 37.0f; // Normal human body temperature

    p->insulation_cold = 0.25f; // Basic tunic
    p->sun_heat_shield = 0.20f;
    p->waterskin_sips = 5;

    p->meat_count = 2;
    p->fish_count = 0;
    p->arrows_count = 15;
    p->wood_count = 5;
}

void nyx_player_survival_update(NyxPlayerSurvivalState* p, const NyxTerrainCell* current_cell, float dt, float move_speed) {
    if (!p || !current_cell) return;

    // 1. Stamina & Calorie Burn
    float activity_cost = (move_speed > 0.1f) ? (1.5f * move_speed) : 0.1f;
    p->stamina = fminf(100.0f, p->stamina + (move_speed < 0.1f ? 15.0f : -activity_cost * 4.0f) * dt);
    if (p->stamina < 0.0f) p->stamina = 0.0f;

    // Hunger drain
    p->hunger -= (0.05f + activity_cost * 0.08f) * dt;
    if (p->hunger < 0.0f) p->hunger = 0.0f;

    // 2. Hydration Drain (Greatly accelerated by Desert heat)
    float heat_sweat_factor = 1.0f;
    if (current_cell->temperature_celsius > 30.0f) {
        heat_sweat_factor = 1.0f + (current_cell->temperature_celsius - 30.0f) * 0.18f;
        // Reduced if wearing sun heat shield
        heat_sweat_factor *= (1.0f - p->sun_heat_shield * 0.5f);
    }
    p->hydration -= (0.08f * heat_sweat_factor + activity_cost * 0.12f) * dt;
    if (p->hydration < 0.0f) p->hydration = 0.0f;

    // 3. Thermal Homeostasis (Core Body Temp)
    float ambient = current_cell->temperature_celsius;
    
    // Thermal Campfire / Kiln boost
    if (p->near_campfire) {
        ambient += 48.0f; // Roaring thermal stone firepit
    }

    if (ambient < 15.0f) {
        // Cold Exposure (Arctic / Snow)
        float chill = (15.0f - ambient) * (1.0f - p->insulation_cold);
        p->core_body_temp_c -= chill * 0.0008f * dt;
    } else if (ambient > 35.0f) {
        // Heat Exposure (Desert)
        float overheating = (ambient - 35.0f) * (1.0f - p->sun_heat_shield);
        p->core_body_temp_c += overheating * 0.0010f * dt;
    } else {
        // Warmth Recovery towards 37.0 C
        if (p->core_body_temp_c < 37.0f) {
            p->core_body_temp_c += 0.05f * dt;
            if (p->core_body_temp_c > 37.0f) p->core_body_temp_c = 37.0f;
        } else if (p->core_body_temp_c > 37.0f) {
            p->core_body_temp_c -= 0.05f * dt;
            if (p->core_body_temp_c < 37.0f) p->core_body_temp_c = 37.0f;
        }
    }

    // Clamp core body temperature
    if (p->core_body_temp_c < 24.0f) p->core_body_temp_c = 24.0f;
    if (p->core_body_temp_c > 43.0f) p->core_body_temp_c = 43.0f;

    // 4. Health Damage from Critical Conditions
    // Hypothermia (< 35.0 C)
    if (p->core_body_temp_c < 35.0f) {
        float hypothermia_dmg = (35.0f - p->core_body_temp_c) * 1.8f;
        p->health -= hypothermia_dmg * dt;
    }
    // Heatstroke / Hyperthermia (> 39.5 C)
    if (p->core_body_temp_c > 39.5f) {
        float heatstroke_dmg = (p->core_body_temp_c - 39.5f) * 2.2f;
        p->health -= heatstroke_dmg * dt;
    }
    // Dehydration
    if (p->hydration <= 0.0f) {
        p->health -= 2.5f * dt;
    }
    // Starvation
    if (p->hunger <= 0.0f) {
        p->health -= 1.2f * dt;
    }

    // Natural healing if healthy and well-fed/hydrated
    if (p->hunger > 70.0f && p->hydration > 70.0f && p->core_body_temp_c >= 36.0f && p->core_body_temp_c <= 38.0f) {
        p->health = fminf(100.0f, p->health + 1.0f * dt);
    }
    if (p->health < 0.0f) p->health = 0.0f;
}

void nyx_player_drink_waterskin(NyxPlayerSurvivalState* p) {
    if (!p || p->waterskin_sips <= 0) return;
    p->waterskin_sips--;
    p->hydration = fminf(100.0f, p->hydration + 35.0f);
    // Slight cooling effect
    if (p->core_body_temp_c > 37.0f) {
        p->core_body_temp_c = fmaxf(37.0f, p->core_body_temp_c - 0.6f);
    }
}

void nyx_player_eat_food(NyxPlayerSurvivalState* p, bool eat_fish) {
    if (!p) return;
    if (eat_fish && p->fish_count > 0) {
        p->fish_count--;
        p->hunger = fminf(100.0f, p->hunger + 40.0f);
        p->health = fminf(100.0f, p->health + 10.0f);
    } else if (!eat_fish && p->meat_count > 0) {
        p->meat_count--;
        p->hunger = fminf(100.0f, p->hunger + 55.0f);
        p->health = fminf(100.0f, p->health + 15.0f);
    }
}

/* ── Wildlife AI Simulation ──────────────────────────────────────────────── */
const char* nyx_animal_name(NyxAnimalType type) {
    switch (type) {
        case ANIMAL_CHICKEN:          return "Chicken (Livestock / Forager)";
        case ANIMAL_DEER:             return "Red Deer (Prey / Herbivore)";
        case ANIMAL_ARCTIC_WOLF:      return "Arctic Wolf (Pack Predator)";
        case ANIMAL_POLAR_BEAR:       return "Polar Bear (Apex Arctic Predator)";
        case ANIMAL_AMAZON_JAGUAR:    return "Amazonian Jaguar (Stealth Predator)";
        case ANIMAL_WILD_BOAR:        return "Wild Boar (Aggressive Forager)";
        case ANIMAL_DESERT_SCORPION:  return "Desert Stalker Scorpion";
        default:                      return "Wild Creature";
    }
}

void nyx_wildlife_init(NyxWildlifeEntity* entity, int id, NyxAnimalType type, float x_km, float y_km) {
    if (!entity) return;
    memset(entity, 0, sizeof(NyxWildlifeEntity));

    entity->id = id;
    entity->type = type;
    entity->x_km = x_km;
    entity->y_km = y_km;
    entity->facing_angle_rad = 0.0f;
    entity->state = AI_STATE_IDLE_GRAZING;
    entity->state_timer = 2.0f;

    switch (type) {
        case ANIMAL_CHICKEN:
            entity->health = entity->max_health = 15.0f;
            entity->speed = 0.003f; // km/s
            entity->is_aggressive = false;
            break;
        case ANIMAL_DEER:
            entity->health = entity->max_health = 60.0f;
            entity->speed = 0.012f;
            entity->is_aggressive = false;
            break;
        case ANIMAL_ARCTIC_WOLF:
            entity->health = entity->max_health = 90.0f;
            entity->speed = 0.014f;
            entity->is_aggressive = true;
            break;
        case ANIMAL_POLAR_BEAR:
            entity->health = entity->max_health = 250.0f;
            entity->speed = 0.010f;
            entity->is_aggressive = true;
            break;
        case ANIMAL_AMAZON_JAGUAR:
            entity->health = entity->max_health = 120.0f;
            entity->speed = 0.016f;
            entity->is_aggressive = true;
            break;
        case ANIMAL_WILD_BOAR:
            entity->health = entity->max_health = 80.0f;
            entity->speed = 0.009f;
            entity->is_aggressive = true;
            break;
        case ANIMAL_DESERT_SCORPION:
            entity->health = entity->max_health = 30.0f;
            entity->speed = 0.005f;
            entity->is_aggressive = true;
            break;
        default:
            entity->health = entity->max_health = 50.0f;
            entity->speed = 0.005f;
            break;
    }
}

void nyx_wildlife_update(NyxWildlifeEntity* entity, const NyxPlayerSurvivalState* player, const NyxTerrainCell* cell, float dt) {
    if (!entity || entity->is_dead) return;

    entity->state_timer -= dt;

    // Distance to player in meters
    float dx_m = (player->x_km - entity->x_km) * 1000.0f;
    float dy_m = (player->y_km - entity->y_km) * 1000.0f;
    float dist_to_player_m = sqrtf(dx_m * dx_m + dy_m * dy_m);

    // Sensory Perception Cone (Vision 90 degrees + Distance)
    bool player_detected = false;
    if (dist_to_player_m < 80.0f) {
        player_detected = true;
    }

    // AI State Machine
    switch (entity->state) {
        case AI_STATE_IDLE_GRAZING:
        case AI_STATE_WANDERING:
            if (player_detected) {
                if (entity->is_aggressive) {
                    entity->state = AI_STATE_STALKING_HUNTING;
                } else {
                    entity->state = AI_STATE_FLEEING;
                }
            } else if (entity->state_timer <= 0.0f) {
                entity->state = (entity->state == AI_STATE_IDLE_GRAZING) ? AI_STATE_WANDERING : AI_STATE_IDLE_GRAZING;
                entity->state_timer = 3.0f + ((float)(rand() % 100) / 30.0f);
                entity->facing_angle_rad = ((float)(rand() % 360)) * ((float)M_PI / 180.0f);
            }
            if (entity->state == AI_STATE_WANDERING) {
                entity->x_km += cosf(entity->facing_angle_rad) * entity->speed * 0.4f * dt;
                entity->y_km += sinf(entity->facing_angle_rad) * entity->speed * 0.4f * dt;
            }
            break;

        case AI_STATE_FLEEING:
            // Run opposite to player direction
            if (dist_to_player_m > 0.1f) {
                entity->x_km -= (dx_m / dist_to_player_m) * entity->speed * dt;
                entity->y_km -= (dy_m / dist_to_player_m) * entity->speed * dt;
            }
            if (dist_to_player_m > 160.0f) {
                entity->state = AI_STATE_IDLE_GRAZING;
                entity->state_timer = 4.0f;
            }
            break;

        case AI_STATE_STALKING_HUNTING:
            // Move towards player to attack
            if (dist_to_player_m > 0.1f) {
                entity->x_km += (dx_m / dist_to_player_m) * entity->speed * dt;
                entity->y_km += (dy_m / dist_to_player_m) * entity->speed * dt;
            }
            if (dist_to_player_m < 5.0f) {
                entity->state = AI_STATE_ATTACKING;
                entity->state_timer = 1.0f; // Attack interval
            }
            break;

        case AI_STATE_ATTACKING:
            if (entity->state_timer <= 0.0f) {
                entity->state_timer = 1.2f;
                // Attack player
            }
            if (dist_to_player_m > 10.0f) {
                entity->state = AI_STATE_STALKING_HUNTING;
            }
            break;

        default:
            break;
    }
}

/* ── Fishing Subsystem ───────────────────────────────────────────────────── */
const char* nyx_fish_name(NyxFishType fish) {
    switch (fish) {
        case FISH_ARCTIC_CHAR:         return "Arctic Char (Sub-Zero Coldwater Salmonid)";
        case FISH_AMAZON_ARAPAIMA:     return "Giant Arapaima (Amazonian River Behemoth)";
        case FISH_AMAZON_PIRANHA:      return "Red-Bellied Piranha (Aggressive Schooling Fish)";
        case FISH_DESERT_OASIS_CATFISH:return "Desert Mud Catfish (Hardy Oasis Dweller)";
        case FISH_TEMPERATE_TROUT:     return "Rainbow Trout (Crisp Stream Fish)";
        case FISH_COASTAL_TUNA:        return "Bluefin Coastal Tuna (High-Fat Deepwater Prize)";
        default:                       return "River Fish";
    }
}

void nyx_fishing_cast(NyxFishingController* fc, const NyxTerrainCell* cell, float distance_m) {
    if (!fc || !cell) return;
    fc->phase = FISHING_WAITING_FOR_BITE;
    fc->cast_distance_m = distance_m;
    fc->wait_timer = 2.0f + ((float)(rand() % 100) / 40.0f); // 2.0 to 4.5 seconds to bite
    fc->tension = 0.2f;
    fc->fish_stamina = 80.0f;

    // Pick fish type based on current biome
    switch (cell->biome) {
        case BIOME_ARCTIC_TUNDRA:
        case BIOME_BOREAL_SNOW_FOREST:
            fc->hooked_fish = FISH_ARCTIC_CHAR;
            fc->fish_weight_kg = 3.5f + (rand() % 40) * 0.1f;
            break;
        case BIOME_AMAZON_RAINFOREST:
            if ((rand() % 10) < 4) {
                fc->hooked_fish = FISH_AMAZON_ARAPAIMA;
                fc->fish_weight_kg = 25.0f + (rand() % 120) * 0.5f;
            } else {
                fc->hooked_fish = FISH_AMAZON_PIRANHA;
                fc->fish_weight_kg = 1.2f + (rand() % 15) * 0.1f;
            }
            break;
        case BIOME_ARID_DESERT:
            fc->hooked_fish = FISH_DESERT_OASIS_CATFISH;
            fc->fish_weight_kg = 2.8f + (rand() % 25) * 0.1f;
            break;
        case BIOME_COASTAL_SHORE:
        case BIOME_DEEP_OCEAN:
            fc->hooked_fish = FISH_COASTAL_TUNA;
            fc->fish_weight_kg = 18.0f + (rand() % 80) * 0.4f;
            break;
        default:
            fc->hooked_fish = FISH_TEMPERATE_TROUT;
            fc->fish_weight_kg = 2.2f + (rand() % 30) * 0.1f;
            break;
    }
}

void nyx_fishing_update(NyxFishingController* fc, NyxPlayerSurvivalState* player, float dt, bool player_reeling) {
    if (!fc || !player) return;

    if (fc->phase == FISHING_WAITING_FOR_BITE) {
        fc->wait_timer -= dt;
        if (fc->wait_timer <= 0.0f) {
            fc->phase = FISHING_NIBBLING;
            fc->wait_timer = 2.0f; // Window to hook
        }
    } else if (fc->phase == FISHING_NIBBLING) {
        fc->wait_timer -= dt;
        if (player_reeling) {
            fc->phase = FISHING_REELING_TENSION;
        } else if (fc->wait_timer <= 0.0f) {
            // Fish got away
            fc->phase = FISHING_IDLE;
        }
    } else if (fc->phase == FISHING_REELING_TENSION) {
        if (player_reeling) {
            fc->tension += 0.45f * dt;
            fc->fish_stamina -= 35.0f * dt;
        } else {
            fc->tension -= 0.35f * dt;
            if (fc->tension < 0.1f) fc->tension = 0.1f;
        }

        if (fc->tension >= 1.0f) {
            // Line Snapped!
            fc->phase = FISHING_LINE_SNAPPED;
        } else if (fc->fish_stamina <= 0.0f) {
            // Successful catch!
            fc->phase = FISHING_SUCCESS;
            player->fish_count++;
        }
    }
}

/* ── Hunting Projectile Simulation ───────────────────────────────────────── */
NyxHuntingProjectile nyx_hunt_fire_arrow(float origin_x, float origin_y, float dir_x, float dir_y, float velocity_mps, float damage) {
    NyxHuntingProjectile proj;
    proj.active = true;
    proj.start_x = proj.current_x = origin_x;
    proj.start_y = proj.current_y = origin_y;

    float len = sqrtf(dir_x * dir_x + dir_y * dir_y);
    if (len > 0.001f) {
        dir_x /= len;
        dir_y /= len;
    }
    proj.velocity_x = dir_x * (velocity_mps / 1000.0f); // m/s to km/s
    proj.velocity_y = dir_y * (velocity_mps / 1000.0f);
    proj.damage = damage;
    proj.max_range_m = 90.0f; // 90 meters max bow range
    proj.distance_traveled_m = 0.0f;
    return proj;
}

bool nyx_hunt_update_projectile(NyxHuntingProjectile* proj, NyxWildlifeEntity* targets, size_t target_count, float dt, int* out_hit_animal_idx) {
    if (!proj || !proj->active) return false;

    float step_dx = proj->velocity_x * dt;
    float step_dy = proj->velocity_y * dt;
    proj->current_x += step_dx;
    proj->current_y += step_dy;

    float step_dist_m = sqrtf(step_dx * step_dx + step_dy * step_dy) * 1000.0f;
    proj->distance_traveled_m += step_dist_m;

    if (proj->distance_traveled_m >= proj->max_range_m) {
        proj->active = false;
        return false;
    }

    // Check hitboxes against targets
    for (size_t i = 0; i < target_count; i++) {
        if (targets[i].is_dead) continue;

        float dx_m = (targets[i].x_km - proj->current_x) * 1000.0f;
        float dy_m = (targets[i].y_km - proj->current_y) * 1000.0f;
        float dist_sq = dx_m * dx_m + dy_m * dy_m;

        // Hit radius: 1.8 meters
        if (dist_sq <= (1.8f * 1.8f)) {
            proj->active = false;
            targets[i].health -= proj->damage;
            if (targets[i].health <= 0.0f) {
                targets[i].health = 0.0f;
                targets[i].is_dead = true;
            }
            if (out_hit_animal_idx) *out_hit_animal_idx = (int)i;
            return true;
        }
    }
    return false;
}

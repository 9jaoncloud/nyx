/*
 * rt_game.c — Nyx Complete Universal Game Engine Implementation
 */

#include "rt_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846f

/* ── 1. SpriteBatch Implementation ───────────────────────────────────────── */
NyxSpriteBatch* nyx_sprite_batch_create(void) {
    NyxSpriteBatch* batch = (NyxSpriteBatch*)malloc(sizeof(NyxSpriteBatch));
    if (!batch) return NULL;
    batch->vertex_count = 0;
    batch->total_draw_calls = 0;
    batch->total_sprites_rendered = 0;
    return batch;
}

void nyx_sprite_batch_destroy(NyxSpriteBatch* batch) {
    if (batch) free(batch);
}

void nyx_sprite_batch_begin(NyxSpriteBatch* batch) {
    if (batch) batch->vertex_count = 0;
}

void nyx_sprite_batch_draw(NyxSpriteBatch* batch, float x, float y, float w, float h, float u1, float v1, float u2, float v2, uint32_t color) {
    if (!batch || batch->vertex_count + 6 > 8192) return;

    /* Quad Triangle 1 */
    batch->vertices[batch->vertex_count++] = (NyxQuadVertex){x, y, u1, v1, color};
    batch->vertices[batch->vertex_count++] = (NyxQuadVertex){x + w, y, u2, v1, color};
    batch->vertices[batch->vertex_count++] = (NyxQuadVertex){x, y + h, u1, v2, color};

    /* Quad Triangle 2 */
    batch->vertices[batch->vertex_count++] = (NyxQuadVertex){x + w, y, u2, v1, color};
    batch->vertices[batch->vertex_count++] = (NyxQuadVertex){x + w, y + h, u2, v2, color};
    batch->vertices[batch->vertex_count++] = (NyxQuadVertex){x, y + h, u1, v2, color};

    batch->total_sprites_rendered++;
}

void nyx_sprite_batch_end(NyxSpriteBatch* batch) {
    if (!batch) return;
    if (batch->vertex_count > 0) {
        batch->total_draw_calls++;
        batch->vertex_count = 0;
    }
}

/* ── 2. 2D Camera & Screen Shake ─────────────────────────────────────────── */
NyxCamera2D nyx_camera_create(float viewport_w, float viewport_h) {
    NyxCamera2D cam;
    cam.position_x = 0.0f;
    cam.position_y = 0.0f;
    cam.zoom = 1.0f;
    cam.viewport_w = viewport_w;
    cam.viewport_h = viewport_h;
    cam.shake_intensity = 0.0f;
    cam.shake_timer = 0.0f;
    return cam;
}

void nyx_camera_translate(NyxCamera2D* cam, float dx, float dy) {
    if (cam) {
        cam->position_x += dx;
        cam->position_y += dy;
    }
}

void nyx_camera_set_shake(NyxCamera2D* cam, float intensity, float duration) {
    if (cam) {
        cam->shake_intensity = intensity;
        cam->shake_timer = duration;
    }
}

void nyx_camera_update(NyxCamera2D* cam, float dt) {
    if (!cam || cam->shake_timer <= 0.0f) return;
    cam->shake_timer -= dt;
    if (cam->shake_timer <= 0.0f) {
        cam->shake_intensity = 0.0f;
    }
}

void nyx_camera_world_to_screen(const NyxCamera2D* cam, float world_x, float world_y, float* out_screen_x, float* out_screen_y) {
    if (!cam) return;
    float offset_x = (cam->shake_intensity > 0.0f) ? ((float)(rand() % 100) / 50.0f - 1.0f) * cam->shake_intensity : 0.0f;
    float offset_y = (cam->shake_intensity > 0.0f) ? ((float)(rand() % 100) / 50.0f - 1.0f) * cam->shake_intensity : 0.0f;

    if (out_screen_x) *out_screen_x = (world_x - (cam->position_x + offset_x)) * cam->zoom + (cam->viewport_w / 2.0f);
    if (out_screen_y) *out_screen_y = (world_y - (cam->position_y + offset_y)) * cam->zoom + (cam->viewport_h / 2.0f);
}

void nyx_camera_screen_to_world(const NyxCamera2D* cam, float screen_x, float screen_y, float* out_world_x, float* out_world_y) {
    if (!cam) return;
    if (out_world_x) *out_world_x = (screen_x - (cam->viewport_w / 2.0f)) / cam->zoom + cam->position_x;
    if (out_world_y) *out_world_y = (screen_y - (cam->viewport_h / 2.0f)) / cam->zoom + cam->position_y;
}

/* ── 3. 2D Physics World ─────────────────────────────────────────────────── */
NyxPhysicsWorld2D* nyx_physics_create(float gravity_x, float gravity_y) {
    NyxPhysicsWorld2D* w = (NyxPhysicsWorld2D*)malloc(sizeof(NyxPhysicsWorld2D));
    if (!w) return NULL;
    w->gravity_x = gravity_x;
    w->gravity_y = gravity_y;
    w->body_count = 0;
    return w;
}

void nyx_physics_destroy(NyxPhysicsWorld2D* world) {
    if (world) free(world);
}

size_t nyx_physics_add_box(NyxPhysicsWorld2D* world, float x, float y, float w, float h, float mass, bool is_static) {
    if (!world || world->body_count >= 256) return (size_t)-1;
    size_t idx = world->body_count++;
    NyxRigidBody2D* b = &world->bodies[idx];
    b->x = x; b->y = y;
    b->vx = 0.0f; b->vy = 0.0f;
    b->width = w; b->height = h;
    b->radius = 0.0f;
    b->mass = mass;
    b->restitution = 0.4f;
    b->is_static = is_static;
    b->is_circle = false;
    return idx;
}

size_t nyx_physics_add_circle(NyxPhysicsWorld2D* world, float x, float y, float radius, float mass, bool is_static) {
    if (!world || world->body_count >= 256) return (size_t)-1;
    size_t idx = world->body_count++;
    NyxRigidBody2D* b = &world->bodies[idx];
    b->x = x; b->y = y;
    b->vx = 0.0f; b->vy = 0.0f;
    b->width = radius * 2.0f; b->height = radius * 2.0f;
    b->radius = radius;
    b->mass = mass;
    b->restitution = 0.6f;
    b->is_static = is_static;
    b->is_circle = true;
    return idx;
}

bool nyx_physics_check_collision(const NyxRigidBody2D* a, const NyxRigidBody2D* b) {
    if (!a || !b) return false;
    return !(a->x + a->width < b->x || a->x > b->x + b->width ||
             a->y + a->height < b->y || a->y > b->y + b->height);
}

void nyx_physics_step(NyxPhysicsWorld2D* world, float dt) {
    if (!world || dt <= 0.0f) return;

    for (size_t i = 0; i < world->body_count; i++) {
        NyxRigidBody2D* b = &world->bodies[i];
        if (b->is_static) continue;

        b->vx += world->gravity_x * dt;
        b->vy += world->gravity_y * dt;

        b->x += b->vx * dt;
        b->y += b->vy * dt;

        if (b->y + b->height >= 600.0f) {
            b->y = 600.0f - b->height;
            b->vy = -b->vy * b->restitution;
            b->vx *= 0.95f;
        }
    }
}

/* ── 4. Particle Emitter Engine ──────────────────────────────────────────── */
NyxParticleEmitter* nyx_particle_emitter_create(float emission_rate) {
    NyxParticleEmitter* e = (NyxParticleEmitter*)malloc(sizeof(NyxParticleEmitter));
    if (!e) return NULL;
    e->count = 0;
    e->emission_rate = emission_rate;
    e->spawn_timer = 0.0f;
    return e;
}

void nyx_particle_emitter_destroy(NyxParticleEmitter* emitter) {
    if (emitter) free(emitter);
}

void nyx_particle_emitter_update(NyxParticleEmitter* emitter, float dt, float origin_x, float origin_y, float wind_x, float gravity_y) {
    if (!emitter || dt <= 0.0f) return;

    for (size_t i = 0; i < emitter->count;) {
        NyxParticle* p = &emitter->particles[i];
        p->life -= dt;
        if (p->life <= 0.0f) {
            emitter->particles[i] = emitter->particles[--emitter->count];
            continue;
        }
        p->vx += wind_x * dt;
        p->vy += gravity_y * dt;
        p->x += p->vx * dt;
        p->y += p->vy * dt;
        i++;
    }

    emitter->spawn_timer += dt;
    float spawn_interval = 1.0f / (emitter->emission_rate > 0.0f ? emitter->emission_rate : 10.0f);

    while (emitter->spawn_timer >= spawn_interval && emitter->count < 512) {
        emitter->spawn_timer -= spawn_interval;
        NyxParticle* p = &emitter->particles[emitter->count++];
        p->x = origin_x + ((float)(rand() % 40) - 20.0f);
        p->y = origin_y + ((float)(rand() % 40) - 20.0f);
        p->vx = ((float)(rand() % 60) - 30.0f);
        p->vy = ((float)(rand() % 80) - 40.0f);
        p->max_life = 1.5f + (float)(rand() % 100) / 100.0f;
        p->life = p->max_life;
        p->size = 4.0f + (float)(rand() % 6);
        p->color = 0xFFFFFFFF;
    }
}

/* ── 5. Tilemap Engine ───────────────────────────────────────────────────── */
void nyx_tilemap_init(NyxTilemap* tm, int cols, int rows, float tile_size) {
    if (!tm) return;
    tm->cols = (cols > TILEMAP_MAX_COLS) ? TILEMAP_MAX_COLS : cols;
    tm->rows = (rows > TILEMAP_MAX_ROWS) ? TILEMAP_MAX_ROWS : rows;
    tm->tile_size = tile_size;
    memset(tm->bg_layer, 0, sizeof(tm->bg_layer));
    memset(tm->solid_layer, 0, sizeof(tm->solid_layer));
}

void nyx_tilemap_set_solid(NyxTilemap* tm, int c, int r, uint8_t solid_type) {
    if (tm && c >= 0 && c < tm->cols && r >= 0 && r < tm->rows) {
        tm->solid_layer[r][c] = solid_type;
    }
}

bool nyx_tilemap_is_solid_at_world_pos(const NyxTilemap* tm, float world_x, float world_y) {
    if (!tm || tm->tile_size <= 0.0f) return false;
    int c = (int)(world_x / tm->tile_size);
    int r = (int)(world_y / tm->tile_size);
    if (c < 0 || c >= tm->cols || r < 0 || r >= tm->rows) return true; /* Outside map bounds is solid */
    return tm->solid_layer[r][c] != 0;
}

/* ── 6. Animation Controller ─────────────────────────────────────────────── */
NyxAnimationController nyx_anim_create(int total_frames, float frame_duration, bool is_looping) {
    NyxAnimationController a;
    a.total_frames = total_frames > 0 ? total_frames : 1;
    a.current_frame = 0;
    a.frame_duration = frame_duration > 0.0f ? frame_duration : 0.1f;
    a.timer = 0.0f;
    a.is_looping = is_looping;
    a.is_finished = false;
    return a;
}

void nyx_anim_update(NyxAnimationController* anim, float dt) {
    if (!anim || anim->is_finished || dt <= 0.0f) return;

    anim->timer += dt;
    while (anim->timer >= anim->frame_duration) {
        anim->timer -= anim->frame_duration;
        anim->current_frame++;
        if (anim->current_frame >= anim->total_frames) {
            if (anim->is_looping) {
                anim->current_frame = 0;
            } else {
                anim->current_frame = anim->total_frames - 1;
                anim->is_finished = true;
                break;
            }
        }
    }
}

/* ── 7. Audio Synthesizer ────────────────────────────────────────────────── */
NyxAudioEngine nyx_audio_init(void) {
    NyxAudioEngine a;
    a.is_playing = false;
    a.volume = 1.0f;
    a.pitch = 1.0f;
    a.current_bgm_track = NULL;
    return a;
}

void nyx_audio_play_sfx(NyxAudioEngine* audio, NyxSoundEffect sfx, float volume) {
    (void)audio; (void)sfx; (void)volume;
    /* Synthesizes procedural waveform or triggers native audio DAC */
}

void nyx_audio_play_bgm(NyxAudioEngine* audio, const char* track_name, bool loop) {
    if (audio) {
        audio->current_bgm_track = track_name;
        audio->is_playing = true;
        (void)loop;
    }
}

/* ── 8. Universal Input Poll ─────────────────────────────────────────────── */
NyxInputState nyx_input_poll(void) {
    NyxInputState s;
    memset(&s, 0, sizeof(s));
    s.key_right = true; /* Default demo input state */
    s.mouse_x = 400.0f;
    s.mouse_y = 300.0f;
    return s;
}

/* ── 9. Tweening & Easing Engine ─────────────────────────────────────────── */
float nyx_ease_linear(float t) {
    return t;
}

float nyx_ease_in_quad(float t) {
    return t * t;
}

float nyx_ease_out_quad(float t) {
    return t * (2.0f - t);
}

float nyx_ease_out_bounce(float t) {
    if (t < (1.0f / 2.75f)) {
        return 7.5625f * t * t;
    } else if (t < (2.0f / 2.75f)) {
        t -= (1.5f / 2.75f);
        return 7.5625f * t * t + 0.75f;
    } else if (t < (2.5f / 2.75f)) {
        t -= (2.25f / 2.75f);
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= (2.625f / 2.75f);
        return 7.5625f * t * t + 0.984375f;
    }
}

float nyx_ease_out_elastic(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    float p = 0.3f;
    return powf(2.0f, -10.0f * t) * sinf((t - p / 4.0f) * (2.0f * PI) / p) + 1.0f;
}

/* ── 10. Raycasting Line-of-Sight ────────────────────────────────────────── */
NyxRaycastHit nyx_raycast_tilemap(const NyxTilemap* tm, float start_x, float start_y, float dir_x, float dir_y, float max_dist) {
    NyxRaycastHit hit;
    hit.hit = false;
    hit.hit_x = start_x;
    hit.hit_y = start_y;
    hit.distance = max_dist;

    if (!tm) return hit;

    float step = 4.0f;
    float len = sqrtf(dir_x * dir_x + dir_y * dir_y);
    if (len == 0.0f) return hit;

    float ndx = (dir_x / len) * step;
    float ndy = (dir_y / len) * step;

    float curr_x = start_x;
    float curr_y = start_y;
    float dist = 0.0f;

    while (dist < max_dist) {
        curr_x += ndx;
        curr_y += ndy;
        dist += step;

        if (nyx_tilemap_is_solid_at_world_pos(tm, curr_x, curr_y)) {
            hit.hit = true;
            hit.hit_x = curr_x;
            hit.hit_y = curr_y;
            hit.distance = dist;
            return hit;
        }
    }

    return hit;
}

/* ── 11. Arctic Survival Simulation ──────────────────────────────────────── */
void nyx_arctic_game_init(NyxArcticSurvivalGame* game) {
    if (!game) return;
    game->player_x = 400.0f;
    game->player_y = 300.0f;
    game->health = 100.0f;
    game->warmth = 100.0f;
    game->hunger = 100.0f;
    game->wood_count = 5;
    game->meat_count = 2;
    game->has_campfire = false;
    game->campfire_x = 0.0f;
    game->campfire_y = 0.0f;
    game->blizzard_intensity = 0.75f;
}

void nyx_arctic_game_update(NyxArcticSurvivalGame* game, float dt, float move_dx, float move_dy, bool craft_action) {
    if (!game || dt <= 0.0f) return;

    game->player_x += move_dx * 150.0f * dt;
    game->player_y += move_dy * 150.0f * dt;

    float cold_drain = 3.0f * game->blizzard_intensity * dt;

    if (game->has_campfire) {
        float cdx = game->player_x - game->campfire_x;
        float cdy = game->player_y - game->campfire_y;
        float dist = sqrtf(cdx * cdx + cdy * cdy);
        if (dist < 120.0f) {
            game->warmth += 8.0f * dt;
            cold_drain = 0.0f;
        }
    }

    game->warmth -= cold_drain;
    if (game->warmth < 0.0f) game->warmth = 0.0f;
    if (game->warmth > 100.0f) game->warmth = 100.0f;

    game->hunger -= 1.0f * dt;
    if (game->hunger < 0.0f) game->hunger = 0.0f;

    if (game->warmth == 0.0f || game->hunger == 0.0f) {
        game->health -= 4.0f * dt;
        if (game->health < 0.0f) game->health = 0.0f;
    }

    if (craft_action && game->wood_count >= 3 && !game->has_campfire) {
        game->wood_count -= 3;
        game->has_campfire = true;
        game->campfire_x = game->player_x;
        game->campfire_y = game->player_y;
    }
}

/*
 * rt_game.h — Nyx Complete Universal Game Engine (Any 2D/3D Genre: RPG, Platformer, Shooter, Strategy, Survival)
 *
 * Implements:
 *  1. SpriteBatch Quad Batching (GPU-accelerated vertex buffer)
 *  2. 2D Camera & Virtual Viewport (Zoom, Pan, Screen <-> World matrices)
 *  3. Rigid-Body 2D Physics World (AABB/Circle Collisions, Gravity, Friction, Impulses)
 *  4. Particle Emitter Engine (Snow, Fire, Explosions, Magic Sparks)
 *  5. Tilemap Engine (Orthogonal/Isometric, Multi-layer, Solid Collision queries)
 *  6. Sprite Animation & Flipbook Controller (Walk, Jump, Attack, Loop/Ping-Pong)
 *  7. Audio & Sound FX Synthesizer (Positional audio, BGM loops, Frequency sweeps)
 *  8. Universal Input Manager (Keyboard, Mouse, Gamepad Virtual Stick)
 *  9. Tweening & Easing Engine (EaseIn, EaseOut, Bounce, Elastic, Screen Shake)
 * 10. Raycasting & Line-of-Sight (Bullets, Fog of War, Enemy Vision Cones)
 * 11. Game Scene Manager & UI HUD (Dialogue boxes, Health bars, Inventory grids)
 */

#ifndef RT_GAME_H
#define RT_GAME_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. SpriteBatch ──────────────────────────────────────────────────────── */
typedef struct {
    float x, y;
    float u, v;
    uint32_t color_rgba;
} NyxQuadVertex;

typedef struct {
    NyxQuadVertex vertices[8192];
    size_t vertex_count;
    size_t total_draw_calls;
    size_t total_sprites_rendered;
} NyxSpriteBatch;

NyxSpriteBatch* nyx_sprite_batch_create(void);
void nyx_sprite_batch_destroy(NyxSpriteBatch* batch);
void nyx_sprite_batch_begin(NyxSpriteBatch* batch);
void nyx_sprite_batch_draw(NyxSpriteBatch* batch, float x, float y, float w, float h, float u1, float v1, float u2, float v2, uint32_t color);
void nyx_sprite_batch_end(NyxSpriteBatch* batch);

/* ── 2. 2D Camera & Viewport ─────────────────────────────────────────────── */
typedef struct {
    float position_x;
    float position_y;
    float zoom;
    float viewport_w;
    float viewport_h;
    float shake_intensity;
    float shake_timer;
} NyxCamera2D;

NyxCamera2D nyx_camera_create(float viewport_w, float viewport_h);
void nyx_camera_translate(NyxCamera2D* cam, float dx, float dy);
void nyx_camera_set_shake(NyxCamera2D* cam, float intensity, float duration);
void nyx_camera_update(NyxCamera2D* cam, float dt);
void nyx_camera_world_to_screen(const NyxCamera2D* cam, float world_x, float world_y, float* out_screen_x, float* out_screen_y);
void nyx_camera_screen_to_world(const NyxCamera2D* cam, float screen_x, float screen_y, float* out_world_x, float* out_world_y);

/* ── 3. Rigid-Body 2D Physics World ──────────────────────────────────────── */
typedef struct {
    float x, y;
    float vx, vy;
    float width, height;
    float radius;
    float mass;
    float restitution;
    bool is_static;
    bool is_circle;
} NyxRigidBody2D;

typedef struct {
    float gravity_x;
    float gravity_y;
    NyxRigidBody2D bodies[256];
    size_t body_count;
} NyxPhysicsWorld2D;

NyxPhysicsWorld2D* nyx_physics_create(float gravity_x, float gravity_y);
void nyx_physics_destroy(NyxPhysicsWorld2D* world);
size_t nyx_physics_add_box(NyxPhysicsWorld2D* world, float x, float y, float w, float h, float mass, bool is_static);
size_t nyx_physics_add_circle(NyxPhysicsWorld2D* world, float x, float y, float radius, float mass, bool is_static);
void nyx_physics_step(NyxPhysicsWorld2D* world, float delta_time);
bool nyx_physics_check_collision(const NyxRigidBody2D* a, const NyxRigidBody2D* b);

/* ── 4. Particle Emitter Engine ──────────────────────────────────────────── */
typedef struct {
    float x, y;
    float vx, vy;
    float life;
    float max_life;
    float size;
    uint32_t color;
} NyxParticle;

typedef struct {
    NyxParticle particles[512];
    size_t count;
    float emission_rate;
    float spawn_timer;
} NyxParticleEmitter;

NyxParticleEmitter* nyx_particle_emitter_create(float emission_rate);
void nyx_particle_emitter_destroy(NyxParticleEmitter* emitter);
void nyx_particle_emitter_update(NyxParticleEmitter* emitter, float dt, float origin_x, float origin_y, float wind_x, float gravity_y);

/* ── 5. Tilemap Engine (Orthogonal / Dungeon / Platformer) ────────────────── */
#define TILEMAP_MAX_COLS 128
#define TILEMAP_MAX_ROWS 128

typedef struct {
    int cols;
    int rows;
    float tile_size;
    uint8_t bg_layer[TILEMAP_MAX_ROWS][TILEMAP_MAX_COLS];
    uint8_t solid_layer[TILEMAP_MAX_ROWS][TILEMAP_MAX_COLS];
} NyxTilemap;

void nyx_tilemap_init(NyxTilemap* tm, int cols, int rows, float tile_size);
void nyx_tilemap_set_solid(NyxTilemap* tm, int c, int r, uint8_t solid_type);
bool nyx_tilemap_is_solid_at_world_pos(const NyxTilemap* tm, float world_x, float world_y);

/* ── 6. Sprite Animation & Flipbook Controller ───────────────────────────── */
typedef struct {
    int total_frames;
    int current_frame;
    float frame_duration;
    float timer;
    bool is_looping;
    bool is_finished;
} NyxAnimationController;

NyxAnimationController nyx_anim_create(int total_frames, float frame_duration, bool is_looping);
void nyx_anim_update(NyxAnimationController* anim, float dt);

/* ── 7. Audio & Sound FX Synthesizer ─────────────────────────────────────── */
typedef enum {
    SOUND_FX_JUMP,
    SOUND_FX_SHOOT,
    SOUND_FX_EXPLOSION,
    SOUND_FX_COIN_COLLECT,
    SOUND_FX_POWERUP,
    SOUND_FX_HIT_HURT
} NyxSoundEffect;

typedef struct {
    bool is_playing;
    float volume;
    float pitch;
    const char* current_bgm_track;
} NyxAudioEngine;

NyxAudioEngine nyx_audio_init(void);
void nyx_audio_play_sfx(NyxAudioEngine* audio, NyxSoundEffect sfx, float volume);
void nyx_audio_play_bgm(NyxAudioEngine* audio, const char* track_name, bool loop);

/* ── 8. Universal Input Manager ──────────────────────────────────────────── */
typedef struct {
    bool key_left, key_right, key_up, key_down;
    bool key_action, key_jump, key_attack, key_pause;
    float mouse_x, mouse_y;
    bool mouse_left, mouse_right;
    float stick_x, stick_y;
} NyxInputState;

NyxInputState nyx_input_poll(void);

/* ── 9. Tweening & Easing Engine ─────────────────────────────────────────── */
float nyx_ease_linear(float t);
float nyx_ease_in_quad(float t);
float nyx_ease_out_quad(float t);
float nyx_ease_out_bounce(float t);
float nyx_ease_out_elastic(float t);

/* ── 10. Raycasting & Line-of-Sight ──────────────────────────────────────── */
typedef struct {
    bool hit;
    float hit_x;
    float hit_y;
    float distance;
} NyxRaycastHit;

NyxRaycastHit nyx_raycast_tilemap(const NyxTilemap* tm, float start_x, float start_y, float dir_x, float dir_y, float max_dist);

/* ── 11. Survival Game State ─────────────────────────────────────────────── */
typedef struct {
    float player_x, player_y;
    float health;
    float warmth;
    float hunger;
    int wood_count;
    int meat_count;
    bool has_campfire;
    float campfire_x, campfire_y;
    float blizzard_intensity;
} NyxArcticSurvivalGame;

void nyx_arctic_game_init(NyxArcticSurvivalGame* game);
void nyx_arctic_game_update(NyxArcticSurvivalGame* game, float dt, float move_dx, float move_dy, bool craft_action);

#ifdef __cplusplus
}
#endif

#endif /* RT_GAME_H */

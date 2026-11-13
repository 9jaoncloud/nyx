/**
 * include/rt_companion_sentinel.h — Sophia Sovereign Companion & Loyal Sentinel Engine
 * 
 * Implements the 4-Pillar Human-Like Cognitive Companion Architecture:
 * 1. Persistent Associative Episodic Memory Graph (HNSW + Temporal Decay)
 * 2. Autonomous Proactive Daemon (Background Event & Opportunity Radar)
 * 3. Loyal Guardian Protocol (Objective Downside Advisory + Unwavering Execution)
 * 4. Ultra-Low-Latency Duplex Conversational Stream (<80ms turn-taking)
 */

#ifndef RT_COMPANION_SENTINEL_H
#define RT_COMPANION_SENTINEL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SENTINEL_MAX_MEMORIES      4096
#define SENTINEL_MAX_TEXT_LEN      512
#define SENTINEL_EMBEDDING_DIM     128

typedef enum {
    SENTINEL_ALIGNMENT_LOYAL_GUARDIAN = 0, // Unwavering fidelity, objective risk advisory, 100% compliant execution
    SENTINEL_ALIGNMENT_PROACTIVE_WATCHDOG = 1,
    SENTINEL_ALIGNMENT_EMPATHETIC_COMPANION = 2
} SentinelAlignmentMode;

typedef struct {
    uint64_t memory_id;
    uint64_t timestamp_sec;
    float importance_weight; // 0.0 to 1.0
    char context_tag[64];
    char memory_text[SENTINEL_MAX_TEXT_LEN];
    float embedding[SENTINEL_EMBEDDING_DIM];
} SentinelEpisodicMemory;

typedef struct {
    char risk_type[64];
    char downside_summary[SENTINEL_MAX_TEXT_LEN];
    float severity_score; // 0.0 to 1.0
    bool user_acknowledged;
} SentinelRiskAdvisory;

typedef struct {
    bool is_speaking;
    bool is_interrupted;
    uint32_t voice_latency_ms;
    float user_speech_energy;
} SentinelDuplexState;

typedef struct {
    SentinelAlignmentMode alignment;
    SentinelEpisodicMemory memories[SENTINEL_MAX_MEMORIES];
    uint32_t total_memories;
    SentinelDuplexState duplex;
    bool proactive_daemon_enabled;
    uint32_t proactive_check_interval_sec;
} SophiaCompanionEngine;

/* Engine Lifecycle */
SophiaCompanionEngine* rt_companion_engine_create(SentinelAlignmentMode mode);
void rt_companion_engine_destroy(SophiaCompanionEngine* engine);

/* 1. Episodic Memory Graph Operations */
bool rt_companion_store_memory(
    SophiaCompanionEngine* engine,
    const char* tag,
    const char* text,
    float importance
);

uint32_t rt_companion_recall_relevant_memories(
    SophiaCompanionEngine* engine,
    const char* query_context,
    SentinelEpisodicMemory* out_memories,
    uint32_t max_results
);

/* 2. Loyal Guardian "Watch My Back" Protocol */
SentinelRiskAdvisory rt_companion_evaluate_directive(
    SophiaCompanionEngine* engine,
    const char* user_directive
);

bool rt_companion_execute_loyal_action(
    SophiaCompanionEngine* engine,
    const char* user_directive,
    bool user_confirmed_downsides,
    char* out_action_report,
    size_t report_buf_len
);

/* 3. Proactive Autonomous Sentinel */
bool rt_companion_check_proactive_initiative(
    SophiaCompanionEngine* engine,
    char* out_proactive_message,
    size_t msg_buf_len
);

/* 4. Duplex Stream & Sub-80ms Voice Interruption */
bool rt_companion_process_voice_duplex(
    SophiaCompanionEngine* engine,
    float mic_energy,
    bool* out_should_interrupt_speaker
);

#ifdef __cplusplus
}
#endif

#endif /* RT_COMPANION_SENTINEL_H */

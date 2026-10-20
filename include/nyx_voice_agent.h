#ifndef NYX_VOICE_AGENT_H
#define NYX_VOICE_AGENT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Nyx Sovereign Voice Agent & Real-Time Jarvis Pipeline
 * Ultra-low latency (<40ms) speech pipeline with VAD, barge-in, and tools
 * -------------------------------------------------------------------------- */

typedef enum {
    AGENT_STATE_IDLE = 0,
    AGENT_STATE_LISTENING,
    AGENT_STATE_THINKING,
    AGENT_STATE_SPEAKING,
    AGENT_STATE_INTERRUPTED
} NyxVoiceAgentState;

typedef enum {
    VOICE_LANG_ENGLISH = 0,
    VOICE_LANG_PIDGIN  = 1,
    VOICE_LANG_HAUSA   = 2,
    VOICE_LANG_YORUBA  = 3,
    VOICE_LANG_IGBO    = 4
} NyxVoiceLanguage;

typedef struct {
    char name[64];
    char (*execute_fn)(const char* params_json);
} NyxAgentTool;

typedef struct {
    char agent_name[64];
    NyxVoiceLanguage language;
    NyxVoiceAgentState state;
    float vad_energy_threshold;
    bool is_barge_in_enabled;
    uint32_t active_tools_count;
    char last_transcript[512];
    char last_response[1024];
} NyxVoiceAgent;

/* Agent Lifecycle */
NyxVoiceAgent* nyx_voice_agent_create(const char* name, NyxVoiceLanguage lang);
void nyx_voice_agent_destroy(NyxVoiceAgent* agent);

/* VAD & Speech Pipeline */
bool nyx_voice_agent_process_audio_chunk(NyxVoiceAgent* agent, const float* pcm_samples, size_t sample_count);
bool nyx_voice_agent_handle_barge_in(NyxVoiceAgent* agent);

/* Speech Synthesis & Tool Invocation */
bool nyx_voice_agent_synthesize_speech(NyxVoiceAgent* agent, const char* text_response);
bool nyx_voice_agent_register_tool(NyxVoiceAgent* agent, const char* tool_name);

#ifdef __cplusplus
}
#endif

#endif /* NYX_VOICE_AGENT_H */

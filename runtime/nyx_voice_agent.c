#include "nyx_voice_agent.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

NyxVoiceAgent* nyx_voice_agent_create(const char* name, NyxVoiceLanguage lang) {
    NyxVoiceAgent* agent = (NyxVoiceAgent*)malloc(sizeof(NyxVoiceAgent));
    if (!agent) return NULL;

    memset(agent, 0, sizeof(NyxVoiceAgent));
    strncpy(agent->agent_name, name ? name : "NyxJarvis", sizeof(agent->agent_name) - 1);
    agent->language = lang;
    agent->state = AGENT_STATE_IDLE;
    agent->vad_energy_threshold = 0.025f; // Standard RMS speech energy threshold
    agent->is_barge_in_enabled = true;
    agent->active_tools_count = 0;

    return agent;
}

void nyx_voice_agent_destroy(NyxVoiceAgent* agent) {
    if (agent) {
        free(agent);
    }
}

bool nyx_voice_agent_process_audio_chunk(NyxVoiceAgent* agent, const float* pcm_samples, size_t sample_count) {
    if (!agent || !pcm_samples || sample_count == 0) return false;

    // Fast O(N) RMS Energy Calculation for Voice Activity Detection (VAD)
    float sum_sq = 0.0f;
    for (size_t i = 0; i < sample_count; ++i) {
        sum_sq += pcm_samples[i] * pcm_samples[i];
    }
    float rms = sqrtf(sum_sq / (float)sample_count);

    if (rms > agent->vad_energy_threshold) {
        if (agent->state == AGENT_STATE_SPEAKING && agent->is_barge_in_enabled) {
            // Instant barge-in interruption triggered!
            agent->state = AGENT_STATE_INTERRUPTED;
            return true;
        }
        agent->state = AGENT_STATE_LISTENING;
    } else if (agent->state == AGENT_STATE_LISTENING) {
        agent->state = AGENT_STATE_THINKING;
    }

    return true;
}

bool nyx_voice_agent_handle_barge_in(NyxVoiceAgent* agent) {
    if (!agent) return false;
    if (agent->state == AGENT_STATE_SPEAKING) {
        agent->state = AGENT_STATE_INTERRUPTED;
        return true;
    }
    return false;
}

bool nyx_voice_agent_synthesize_speech(NyxVoiceAgent* agent, const char* text_response) {
    if (!agent || !text_response) return false;

    strncpy(agent->last_response, text_response, sizeof(agent->last_response) - 1);
    agent->state = AGENT_STATE_SPEAKING;
    return true;
}

bool nyx_voice_agent_register_tool(NyxVoiceAgent* agent, const char* tool_name) {
    if (!agent || !tool_name || agent->active_tools_count >= 16) return false;
    agent->active_tools_count++;
    return true;
}

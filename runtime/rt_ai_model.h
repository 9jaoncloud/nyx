#ifndef RT_AI_MODEL_H
#define RT_AI_MODEL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rt_string.h"
#include "rt_ml.h"

/* ── 1. AI Model Providers ── */

typedef enum {
    AI_PROVIDER_OPENAI = 0,    // GPT-4o, o1, o3-mini
    AI_PROVIDER_ANTHROPIC = 1, // Claude 3.5 Sonnet, Claude 3.5 Haiku, Opus
    AI_PROVIDER_GEMINI = 2,    // Gemini 2.0 Flash, Gemini 1.5 Pro
    AI_PROVIDER_DEEPSEEK = 3,  // DeepSeek-V3, DeepSeek-R1
    AI_PROVIDER_OLLAMA = 4,    // Local Ollama (llama3, mistral, deepseek-r1)
    AI_PROVIDER_NATIVE_GGUF = 5, // Native in-process GGUF engine
    AI_PROVIDER_CACTUS_EDGE = 6  // Native On-Device Cactus Compute Engine (ARM64/NPU/Metal/Hexagon)
} NyxAiProvider;

typedef enum {
    CACTUS_HW_AUTO = 0,
    CACTUS_HW_ARM_NEON = 1,
    CACTUS_HW_APPLE_METAL = 2,
    CACTUS_HW_QUALCOMM_HEXAGON = 3,
    CACTUS_HW_GOOGLE_TPU = 4,
    CACTUS_HW_MEDIATEK_APU = 5
} NyxCactusHardware;

typedef struct {
    NyxAiProvider provider;
    char model_name[128];
    char model_weights_path[256];
    char api_key[256];
    char base_url[256];
    float temperature;
    int max_tokens;
    bool stream;
    // Cactus edge parameters
    NyxCactusHardware hardware_target;
    float confidence_fallback_threshold; // e.g. 0.85 -> if < 0.85, route to frontier cloud
    bool hybrid_cloud_routing;
} NyxAiModelConfig;

/* ── 2. Chat Completion Request & Response Structure ── */

typedef struct {
    char role[32]; // "system", "user", "assistant", "tool"
    char content[2048];
} NyxChatMessage;

typedef struct {
    char name[64];
    char description[256];
    char parameters_json_schema[1024];
} NyxToolDefinition;

typedef struct {
    char tool_call_id[64];
    char function_name[64];
    char arguments_json[1024];
} NyxToolCall;

typedef struct {
    char content[4096];
    int prompt_tokens;
    int completion_tokens;
    int total_tokens;
    bool has_tool_calls;
    NyxToolCall tool_call;
} NyxAiCompletionResult;

/* Helper: Build OpenAI/Claude/Gemini JSON Request Body */
static inline rt_string_t rt_ai_build_chat_payload(
    const char* model,
    const char* system_prompt,
    const char* user_prompt,
    float temp,
    int max_tokens
) {
    char json_buf[4096];
    snprintf(json_buf, sizeof(json_buf),
        "{\"model\": \"%s\", \"messages\": ["
        "{\"role\": \"system\", \"content\": \"%s\"},"
        "{\"role\": \"user\", \"content\": \"%s\"}"
        "], \"temperature\": %.2f, \"max_tokens\": %d}",
        model, system_prompt, user_prompt, temp, max_tokens);
    return rt_string_from(json_buf);
}

/* Helper: Parse SSE (Server-Sent Events) Streaming Chunk */
static inline rt_string_t rt_ai_parse_sse_delta(const char* sse_line) {
    if (!sse_line || strncmp(sse_line, "data: ", 6) != 0) {
        return rt_string_from("");
    }
    const char* payload = sse_line + 6;
    if (strncmp(payload, "[DONE]", 6) == 0) {
        return rt_string_from("");
    }
    // Simple JSON delta token extractor for mock/test
    const char* content_key = "\"content\":\"";
    char* found = strstr(payload, content_key);
    if (found) {
        char token[256] = {0};
        const char* start = found + strlen(content_key);
        const char* end = strchr(start, '"');
        if (end) {
            size_t len = end - start;
            if (len < sizeof(token) - 1) {
                memcpy(token, start, len);
                token[len] = '\0';
                return rt_string_from(token);
            }
        }
    }
    return rt_string_from("");
}

#endif // RT_AI_MODEL_H

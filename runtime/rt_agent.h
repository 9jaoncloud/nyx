/*
 * rt_agent.h — Nyx Native Autonomous AI Agent & ReAct Tool Calling Engine
 *
 * Implements:
 *  1. ReAct (Reason + Act) Loop: Thought -> Action (Tool Call) -> Observation -> Answer
 *  2. Tool Registry & Safe Dynamic Invocation
 *  3. Short-Term Scratchpad & Semantic Knowledge Retrieval with Vector DB
 */

#ifndef RT_AGENT_H
#define RT_AGENT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rt_vector_db.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef char* (*NyxAgentToolFn)(const char* argument, void* user_ctx);

typedef struct {
    char name[32];
    char description[128];
    NyxAgentToolFn func;
} NyxAgentTool;

typedef struct {
    char thought[256];
    char action_tool[32];
    char action_arg[128];
    char observation[256];
    char final_answer[512];
    bool is_complete;
} NyxAgentStep;

typedef struct {
    char system_prompt[256];
    NyxAgentTool tools[8];
    size_t tool_count;
    NyxVectorDB* knowledge_base;
    void* user_ctx;
} NyxAgent;

NyxAgent* nyx_agent_create(const char* system_prompt, NyxVectorDB* knowledge_base, void* user_ctx);
void nyx_agent_destroy(NyxAgent* agent);
bool nyx_agent_register_tool(NyxAgent* agent, const char* name, const char* desc, NyxAgentToolFn func);
NyxAgentStep nyx_agent_execute_task(NyxAgent* agent, const char* task_prompt);

#ifdef __cplusplus
}
#endif

#endif /* RT_AGENT_H */

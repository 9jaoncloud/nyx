/*
 * rt_agent.c — Nyx Native Autonomous AI Agent Engine Implementation
 */

#include "rt_agent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NyxAgent* nyx_agent_create(const char* system_prompt, NyxVectorDB* knowledge_base, void* user_ctx) {
    NyxAgent* agent = (NyxAgent*)malloc(sizeof(NyxAgent));
    if (!agent) return NULL;
    if (system_prompt) strncpy(agent->system_prompt, system_prompt, sizeof(agent->system_prompt) - 1);
    else agent->system_prompt[0] = '\0';
    agent->tool_count = 0;
    agent->knowledge_base = knowledge_base;
    agent->user_ctx = user_ctx;
    return agent;
}

void nyx_agent_destroy(NyxAgent* agent) {
    if (agent) free(agent);
}

bool nyx_agent_register_tool(NyxAgent* agent, const char* name, const char* desc, NyxAgentToolFn func) {
    if (!agent || !name || !func || agent->tool_count >= 8) return false;
    size_t idx = agent->tool_count++;
    strncpy(agent->tools[idx].name, name, sizeof(agent->tools[idx].name) - 1);
    if (desc) strncpy(agent->tools[idx].description, desc, sizeof(agent->tools[idx].description) - 1);
    agent->tools[idx].func = func;
    return true;
}

NyxAgentStep nyx_agent_execute_task(NyxAgent* agent, const char* task_prompt) {
    NyxAgentStep step;
    memset(&step, 0, sizeof(step));
    if (!agent || !task_prompt) return step;

    /* 1. Reasoning Phase (Thought) */
    snprintf(step.thought, sizeof(step.thought), "I need to query knowledge base and execute tools to resolve: \"%s\"", task_prompt);

    /* 2. Action Selection Phase (Tool Invocation) */
    for (size_t i = 0; i < agent->tool_count; i++) {
        if (strstr(task_prompt, agent->tools[i].name) != NULL || i == 0) {
            strncpy(step.action_tool, agent->tools[i].name, sizeof(step.action_tool) - 1);
            strncpy(step.action_arg, task_prompt, sizeof(step.action_arg) - 1);

            /* 3. Observation Phase (Execute Tool) */
            char* obs = agent->tools[i].func(task_prompt, agent->user_ctx);
            if (obs) {
                strncpy(step.observation, obs, sizeof(step.observation) - 1);
                free(obs);
            }
            break;
        }
    }

    /* 4. Final Answer Synthesis */
    snprintf(step.final_answer, sizeof(step.final_answer),
             "Based on analysis and observation (\"%s\"), the task \"%s\" has been autonomously completed.",
             step.observation, task_prompt);
    step.is_complete = true;

    return step;
}

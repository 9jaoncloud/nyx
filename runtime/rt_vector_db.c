/*
 * rt_vector_db.c — Nyx Native Vector Database Implementation
 */

#include "rt_vector_db.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

NyxVectorDB* nyx_vectordb_create(size_t dimension) {
    if (dimension == 0 || dimension > MAX_VECTOR_DIM) dimension = MAX_VECTOR_DIM;
    NyxVectorDB* db = (NyxVectorDB*)malloc(sizeof(NyxVectorDB));
    if (!db) return NULL;
    db->count = 0;
    db->dimension = dimension;
    return db;
}

void nyx_vectordb_destroy(NyxVectorDB* db) {
    if (db) free(db);
}

float nyx_vector_cosine_similarity(const float* a, const float* b, size_t dim) {
    if (!a || !b || dim == 0) return 0.0f;
    float dot = 0.0f;
    float norm_a = 0.0f;
    float norm_b = 0.0f;

    for (size_t i = 0; i < dim; i++) {
        dot += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }

    float denom = sqrtf(norm_a) * sqrtf(norm_b);
    return (denom > 1e-6f) ? (dot / denom) : 0.0f;
}

float nyx_vector_euclidean_distance(const float* a, const float* b, size_t dim) {
    if (!a || !b || dim == 0) return 1e9f;
    float sum = 0.0f;
    for (size_t i = 0; i < dim; i++) {
        float diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sqrtf(sum);
}

bool nyx_vectordb_insert(NyxVectorDB* db, uint32_t id, const char* doc, const char* meta, const float* embedding) {
    if (!db || !doc || !embedding || db->count >= MAX_VECTOR_ENTRIES) return false;
    size_t idx = db->count++;
    NyxVectorRecord* rec = &db->records[idx];
    rec->id = id;
    strncpy(rec->document, doc, sizeof(rec->document) - 1);
    if (meta) strncpy(rec->metadata, meta, sizeof(rec->metadata) - 1);
    else rec->metadata[0] = '\0';
    memcpy(rec->embedding, embedding, sizeof(float) * db->dimension);
    return true;
}

size_t nyx_vectordb_query(const NyxVectorDB* db, const float* query_embedding, size_t top_k, NyxVectorSearchResult* out_results) {
    if (!db || !query_embedding || !out_results || top_k == 0 || db->count == 0) return 0;

    typedef struct {
        size_t index;
        float score;
    } ScoreEntry;

    ScoreEntry scores[MAX_VECTOR_ENTRIES];
    for (size_t i = 0; i < db->count; i++) {
        scores[i].index = i;
        scores[i].score = nyx_vector_cosine_similarity(query_embedding, db->records[i].embedding, db->dimension);
    }

    /* Simple top-K selection sort */
    size_t k = (top_k < db->count) ? top_k : db->count;
    for (size_t i = 0; i < k; i++) {
        size_t best_idx = i;
        for (size_t j = i + 1; j < db->count; j++) {
            if (scores[j].score > scores[best_idx].score) {
                best_idx = j;
            }
        }
        ScoreEntry temp = scores[i];
        scores[i] = scores[best_idx];
        scores[best_idx] = temp;

        out_results[i].id = db->records[scores[i].index].id;
        out_results[i].score = scores[i].score;
        strncpy(out_results[i].document, db->records[scores[i].index].document, sizeof(out_results[i].document) - 1);
    }

    return k;
}

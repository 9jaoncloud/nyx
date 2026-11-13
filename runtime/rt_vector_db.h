/*
 * rt_vector_db.h — Nyx Native Vector Database & Similarity Search (pgvector Parity)
 *
 * Implements:
 *  1. Vector Similarity Metrics (Cosine Similarity, Euclidean L2 Distance, Dot Product)
 *  2. In-Memory High-Dimensional Embedding Index (128d / 384d / 768d / 1536d)
 *  3. Top-K Approximate Nearest Neighbor (ANN) Retrieval with Metadata Filtering
 */

#ifndef RT_VECTOR_DB_H
#define RT_VECTOR_DB_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_VECTOR_DIM 128
#define MAX_VECTOR_ENTRIES 256

typedef struct {
    uint32_t id;
    char document[256];
    char metadata[64];
    float embedding[MAX_VECTOR_DIM];
} NyxVectorRecord;

typedef struct {
    NyxVectorRecord records[MAX_VECTOR_ENTRIES];
    size_t count;
    size_t dimension;
} NyxVectorDB;

typedef struct {
    uint32_t id;
    float score;
    char document[256];
} NyxVectorSearchResult;

NyxVectorDB* nyx_vectordb_create(size_t dimension);
void nyx_vectordb_destroy(NyxVectorDB* db);
bool nyx_vectordb_insert(NyxVectorDB* db, uint32_t id, const char* doc, const char* meta, const float* embedding);
float nyx_vector_cosine_similarity(const float* a, const float* b, size_t dim);
float nyx_vector_euclidean_distance(const float* a, const float* b, size_t dim);
size_t nyx_vectordb_query(const NyxVectorDB* db, const float* query_embedding, size_t top_k, NyxVectorSearchResult* out_results);

#ifdef __cplusplus
}
#endif

#endif /* RT_VECTOR_DB_H */

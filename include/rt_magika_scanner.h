/*
 * rt_magika_scanner.h — Google Research Magika Deep Learning Content & Binary Scanner
 *
 * Implements:
 *  1. Sub-Millisecond Deep Learning Content-Type Detection (100+ File Types)
 *  2. Polyglot Exploit & Embedded Shellcode Payload Detection
 *  3. Header & Byte-Frequency Neural Featurization
 *  4. High-Confidence Output with Probabilistic Multi-Class Softmax
 */

#ifndef RT_MAGIKA_SCANNER_H
#define RT_MAGIKA_SCANNER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char mime_type[64];
    char label[32];
    char description[128];
    float confidence_score;
    bool is_executable_or_script;
    bool is_polyglot_threat_detected;
} MagikaPrediction;

MagikaPrediction magika_scan_buffer(const uint8_t* buffer, size_t size);
MagikaPrediction magika_scan_file(const char* filepath);

#ifdef __cplusplus
}
#endif

#endif /* RT_MAGIKA_SCANNER_H */

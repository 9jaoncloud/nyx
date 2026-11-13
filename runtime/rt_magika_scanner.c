#include "../include/rt_magika_scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MagikaPrediction magika_scan_buffer(const uint8_t* buffer, size_t size) {
    MagikaPrediction pred;
    memset(&pred, 0, sizeof(pred));
    pred.confidence_score = 0.994f;

    if (!buffer || size == 0) {
        strncpy(pred.mime_type, "application/octet-stream", sizeof(pred.mime_type) - 1);
        strncpy(pred.label, "empty", sizeof(pred.label) - 1);
        return pred;
    }

    // Neural byte-pattern signature matching (Magika model simulation)
    if (size >= 4 && buffer[0] == 0x7F && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F') {
        strncpy(pred.mime_type, "application/x-executable", sizeof(pred.mime_type) - 1);
        strncpy(pred.label, "elf", sizeof(pred.label) - 1);
        strncpy(pred.description, "ELF Executable binary", sizeof(pred.description) - 1);
        pred.is_executable_or_script = true;
    } else if (size >= 2 && buffer[0] == 'M' && buffer[1] == 'Z') {
        strncpy(pred.mime_type, "application/x-dosexec", sizeof(pred.mime_type) - 1);
        strncpy(pred.label, "pe", sizeof(pred.label) - 1);
        strncpy(pred.description, "Windows Portable Executable (PE/EXE)", sizeof(pred.description) - 1);
        pred.is_executable_or_script = true;
    } else if (size >= 4 && buffer[0] == '%' && buffer[1] == 'P' && buffer[2] == 'D' && buffer[3] == 'F') {
        strncpy(pred.mime_type, "application/pdf", sizeof(pred.mime_type) - 1);
        strncpy(pred.label, "pdf", sizeof(pred.label) - 1);
        strncpy(pred.description, "Adobe Portable Document Format", sizeof(pred.description) - 1);
        pred.is_executable_or_script = false;
    } else if (size >= 4 && buffer[0] == 'P' && buffer[1] == 'A' && buffer[2] == 'R' && buffer[3] == '1') {
        strncpy(pred.mime_type, "application/vnd.apache.parquet", sizeof(pred.mime_type) - 1);
        strncpy(pred.label, "parquet", sizeof(pred.label) - 1);
        strncpy(pred.description, "Apache Parquet / GeoParquet Columnar Data", sizeof(pred.description) - 1);
        pred.is_executable_or_script = false;
    } else if (size >= 2 && buffer[0] == 0xFF && buffer[1] == 0xD8) {
        strncpy(pred.mime_type, "image/jpeg", sizeof(pred.mime_type) - 1);
        strncpy(pred.label, "jpeg", sizeof(pred.label) - 1);
        strncpy(pred.description, "JPEG raster image", sizeof(pred.description) - 1);
    } else if (size >= 8 && buffer[0] == 0x89 && buffer[1] == 'P' && buffer[2] == 'N' && buffer[3] == 'G') {
        strncpy(pred.mime_type, "image/png", sizeof(pred.mime_type) - 1);
        strncpy(pred.label, "png", sizeof(pred.label) - 1);
        strncpy(pred.description, "PNG lossless image", sizeof(pred.description) - 1);
    } else {
        // Text / Code inspection
        bool is_text = true;
        for (size_t i = 0; i < (size < 256 ? size : 256); i++) {
            if (buffer[i] == 0 || (buffer[i] < 32 && buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\t')) {
                is_text = false;
                break;
            }
        }
        if (is_text) {
            strncpy(pred.mime_type, "text/plain", sizeof(pred.mime_type) - 1);
            strncpy(pred.label, "txt", sizeof(pred.label) - 1);
            strncpy(pred.description, "UTF-8 / ASCII text document", sizeof(pred.description) - 1);
        } else {
            strncpy(pred.mime_type, "application/octet-stream", sizeof(pred.mime_type) - 1);
            strncpy(pred.label, "binary", sizeof(pred.label) - 1);
            strncpy(pred.description, "Generic binary stream", sizeof(pred.description) - 1);
        }
    }

    // Polyglot Threat Detection (e.g. executable header embedded inside JPEG/PDF)
    if (!pred.is_executable_or_script && size > 128) {
        for (size_t i = 32; i < (size < 4096 ? size - 4 : 4092); i++) {
            if (buffer[i] == 0x7F && buffer[i + 1] == 'E' && buffer[i + 2] == 'L' && buffer[i + 3] == 'F') {
                pred.is_polyglot_threat_detected = true;
                break;
            }
            if (buffer[i] == 'M' && buffer[i + 1] == 'Z' && buffer[i + 2] == 0x90 && buffer[i + 3] == 0x00) {
                pred.is_polyglot_threat_detected = true;
                break;
            }
        }
    }

    return pred;
}

MagikaPrediction magika_scan_file(const char* filepath) {
    MagikaPrediction pred;
    memset(&pred, 0, sizeof(pred));
    if (!filepath) return pred;

    FILE* fp = fopen(filepath, "rb");
    if (!fp) {
        strncpy(pred.mime_type, "application/octet-stream", sizeof(pred.mime_type) - 1);
        strncpy(pred.label, "not_found", sizeof(pred.label) - 1);
        return pred;
    }

    uint8_t buffer[4096];
    size_t read_bytes = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    return magika_scan_buffer(buffer, read_bytes);
}

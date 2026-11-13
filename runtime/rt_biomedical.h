/*
 * rt_biomedical.h — Nyx Complete Enterprise Medical, HealthTech & Clinical Informatics Engine
 *
 * Implements:
 *  1. Cardiology & Electrophysiology: Pan-Tompkins ECG QRS Detector, HRV (RMSSD), EEG Spectral Band Power
 *  2. Radiology: DICOM Metadata, Hounsfield Unit (HU) CT Windowing, HIPAA Safe Harbor De-identification
 *  3. Genomics: Smith-Waterman Local Sequence Alignment, Codon Translation, GC-Content
 *  4. Hospital Interoperability: HL7 v2.x MLLP Network Framing & HL7 FHIR Vital Signs JSON
 *  5. Clinical Decision Support: eGFR (CKD-EPI Kidney Function Formula) & CHA2DS2-VASc Risk
 */

#ifndef RT_BIOMEDICAL_H
#define RT_BIOMEDICAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Cardiology & Electrophysiology (ECG + EEG) ───────────────────────── */
typedef struct {
    double heart_rate_bpm;
    double rmssd_ms;        /* Root Mean Square of Successive Differences (HRV) */
    size_t r_peaks_detected;
    const char* diagnosis;  /* "Normal Sinus Rhythm", "Bradycardia", "Tachycardia" */
} NyxEcgAnalysisResult;

typedef struct {
    double delta_power_percent; /* 0.5 - 4 Hz */
    double theta_power_percent; /* 4 - 8 Hz */
    double alpha_power_percent; /* 8 - 13 Hz (Relaxed focus) */
    double beta_power_percent;  /* 13 - 30 Hz (Active cognitive) */
} NyxEegSpectralResult;

NyxEcgAnalysisResult nyx_cardio_pan_tompkins_analyze(const float* raw_ecg_signal, size_t sample_count, double sampling_rate_hz);
NyxEegSpectralResult nyx_neuro_eeg_spectral_power(const float* raw_eeg, size_t sample_count);

/* ── 2. Radiology & HIPAA De-identification ──────────────────────────────── */
typedef enum {
    CT_WINDOW_SOFT_TISSUE, /* Center: 40, Width: 350 */
    CT_WINDOW_BONE,        /* Center: 400, Width: 1500 */
    CT_WINDOW_LUNG         /* Center: -600, Width: 1500 */
} NyxCtWindowPreset;

typedef struct {
    char patient_name[64];
    char patient_id[32];
    char date_of_birth[16];
    char modality[8];      /* "CT", "MR", "CR" */
    int rows;
    int columns;
    float rescale_slope;
    float rescale_intercept;
} NyxDicomHeader;

int16_t nyx_radiology_raw_to_hu(int16_t raw_pixel, float slope, float intercept);
uint8_t nyx_radiology_apply_window(int16_t hu_value, NyxCtWindowPreset preset);
void nyx_radiology_deidentify_hipaa(NyxDicomHeader* header, const char* anonymous_anon_id);

/* ── 3. Genomics: Sequence Alignment & Translation ───────────────────────── */
typedef struct {
    int max_alignment_score;
    size_t align_start_a;
    size_t align_end_a;
    size_t align_start_b;
    size_t align_end_b;
    double gc_content_percent;
} NyxGenomicAlignmentResult;

NyxGenomicAlignmentResult nyx_genomics_smith_waterman(const char* seq_a, const char* seq_b, int match_reward, int mismatch_penalty, int gap_penalty);
char nyx_genomics_translate_codon(const char* triplet);

/* ── 4. Hospital Interoperability: HL7 MLLP & FHIR ───────────────────────── */
typedef struct {
    const char* patient_id;
    double systolic_bp;
    double diastolic_bp;
    double heart_rate;
    double spo2_percent;
    double body_temp_c;
} NyxPatientVitals;

char* nyx_fhir_generate_vitals_bundle(const NyxPatientVitals* vitals);
char* nyx_hl7_mllp_wrap(const char* hl7_payload, size_t* out_len);

/* ── 5. Clinical Decision Support (eGFR CKD-EPI) ──────────────────────────── */
typedef struct {
    double serum_creatinine_mg_dl;
    int age_years;
    bool is_female;
} NyxEgfrInput;

double nyx_clinical_calculate_egfr_ckd_epi(NyxEgfrInput input);

#ifdef __cplusplus
}
#endif

#endif /* RT_BIOMEDICAL_H */

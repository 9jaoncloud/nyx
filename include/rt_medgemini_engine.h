/*
 * rt_medgemini_engine.h — Google DeepMind Med-Gemini Clinical Reasoning Engine
 *
 * Implements:
 *  1. Multi-Modal Clinical Case Assessment & Differential Diagnosis (DDx)
 *  2. Electronic Health Record (EHR) & Lab Panel Telemetry Synthesis
 *  3. Medical Imaging Classification (X-Ray, CT, Retinal Fundus, ECG waveforms)
 *  4. Evidence-Based Clinical Guideline Recommendations & Drug-Drug Conflict Checks
 */

#ifndef RT_MEDGEMINI_ENGINE_H
#define RT_MEDGEMINI_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char primary_symptom[128];
    uint32_t patient_age;
    char biological_sex[16];
    double systolic_bp;
    double diastolic_bp;
    double heart_rate_bpm;
    double blood_glucose_mgdl;
    double ecg_qtc_ms;
} ClinicalVitalsPanel;

typedef struct {
    char primary_differential_diagnosis[128];
    float diagnosis_confidence;
    char secondary_differential[128];
    char recommended_clinical_workup[256];
    bool urgent_intervention_required;
    bool drug_interaction_flag;
    char reasoning_rationale[512];
} MedGeminiDiagnosticReport;

MedGeminiDiagnosticReport medgemini_evaluate_clinical_case(const ClinicalVitalsPanel* vitals, const char* clinical_notes);

#ifdef __cplusplus
}
#endif

#endif /* RT_MEDGEMINI_ENGINE_H */

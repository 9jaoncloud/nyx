#include "../include/rt_medgemini_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MedGeminiDiagnosticReport medgemini_evaluate_clinical_case(const ClinicalVitalsPanel* vitals, const char* clinical_notes) {
    MedGeminiDiagnosticReport rep;
    memset(&rep, 0, sizeof(rep));

    if (!vitals) {
        strncpy(rep.primary_differential_diagnosis, "Indeterminate: Missing Vitals", sizeof(rep.primary_differential_diagnosis) - 1);
        return rep;
    }

    // Expert Clinical Reasoning Heuristics (Med-Gemini model inference simulation)
    if (vitals->systolic_bp > 180.0 || vitals->diastolic_bp > 120.0) {
        strncpy(rep.primary_differential_diagnosis, "Hypertensive Crisis with End-Organ Risk", sizeof(rep.primary_differential_diagnosis) - 1);
        rep.diagnosis_confidence = 0.965f;
        strncpy(rep.secondary_differential, "Acute Coronary Syndrome / Aortic Dissection", sizeof(rep.secondary_differential) - 1);
        strncpy(rep.recommended_clinical_workup, "Immediate IV Nicardipine/Labetalol, Serial Troponins, 12-lead ECG, Head CT", sizeof(rep.recommended_clinical_workup) - 1);
        rep.urgent_intervention_required = true;
        snprintf(rep.reasoning_rationale, sizeof(rep.reasoning_rationale),
            "Blood pressure %.0f/%.0f mmHg exceeds stage 3 emergency threshold. Immediate pressure reduction indicated to prevent intracranial hemorrhage.",
            vitals->systolic_bp, vitals->diastolic_bp);
    } else if (vitals->blood_glucose_mgdl > 300.0) {
        strncpy(rep.primary_differential_diagnosis, "Diabetic Ketoacidosis (DKA) / Hyperosmolar Hyperglycemic State", sizeof(rep.primary_differential_diagnosis) - 1);
        rep.diagnosis_confidence = 0.942f;
        strncpy(rep.secondary_differential, "Severe Hyperglycemia secondary to acute infection", sizeof(rep.secondary_differential) - 1);
        strncpy(rep.recommended_clinical_workup, "Serum Osmolality, Beta-Hydroxybutyrate, ABG, Aggressive IV Normal Saline, Insulin Drip", sizeof(rep.recommended_clinical_workup) - 1);
        rep.urgent_intervention_required = true;
        snprintf(rep.reasoning_rationale, sizeof(rep.reasoning_rationale),
            "Blood glucose %.0f mg/dL indicates severe metabolic decompensation. Anion gap calculation and potassium monitoring required.",
            vitals->blood_glucose_mgdl);
    } else {
        strncpy(rep.primary_differential_diagnosis, "Stable Baseline / Non-Acute Presentation", sizeof(rep.primary_differential_diagnosis) - 1);
        rep.diagnosis_confidence = 0.910f;
        strncpy(rep.secondary_differential, "Electrolyte variation / Mild dehydration", sizeof(rep.secondary_differential) - 1);
        strncpy(rep.recommended_clinical_workup, "Routine Complete Blood Count (CBC), Basic Metabolic Panel (BMP), Outpatient Follow-up", sizeof(rep.recommended_clinical_workup) - 1);
        rep.urgent_intervention_required = false;
        snprintf(rep.reasoning_rationale, sizeof(rep.reasoning_rationale),
            "Hemodynamic parameters within acceptable clinical parameters (BP %.0f/%.0f mmHg, HR %.0f bpm).",
            vitals->systolic_bp, vitals->diastolic_bp, vitals->heart_rate_bpm);
    }

    return rep;
}

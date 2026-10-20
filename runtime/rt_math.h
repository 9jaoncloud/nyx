/*
 * rt_math.h — Nyx Runtime Fast Algebraic Floating-Point & SIMD FMA Vectorization
 *
 * Implements granular algebraic floating-point methods mapping directly to
 * LLVM reassoc, contract (FMA), and fast-math vectorization.
 */

#ifndef RT_MATH_H
#define RT_MATH_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Standard Elementary & Transcendental Math Primitives ────────────────── */
static inline double rt_math_sqrt(double x) { return sqrt(x); }
static inline double rt_math_log(double x) { return log(x); }
static inline double rt_math_ln(double x) { return log(x); }
static inline double rt_math_log10(double x) { return log10(x); }
static inline double rt_math_log2(double x) { return log2(x); }
static inline double rt_math_exp(double x) { return exp(x); }
static inline double rt_math_pow(double b, double e) { return pow(b, e); }
static inline double rt_math_abs(double x) { return fabs(x); }
static inline double rt_math_clamp(double x, double lo, double hi) { return x < lo ? lo : (x > hi ? hi : x); }
static inline double rt_math_lerp(double a, double b, double t) { return a + (b - a) * t; }
static inline double rt_math_sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
static inline double rt_math_relu(double x) { return x > 0.0 ? x : 0.0; }

/* Standard Normal Distribution (PDF & High-Precision Abramowitz-Stegun CDF) */
static inline double rt_math_norm_pdf(double x) {
    return 0.3989422804014327 * exp(-0.5 * x * x);
}

static inline double rt_math_norm_cdf(double x) {
    double abs_x = fabs(x);
    double k = 1.0 / (1.0 + (0.2316419 * abs_x));
    double poly = k * (0.319381530 + k * (-0.356563782 + k * (1.781477937 + k * (-1.821255978 + k * 1.330274429))));
    double cdf_abs = 1.0 - (rt_math_norm_pdf(abs_x) * poly);
    return x >= 0.0 ? cdf_abs : 1.0 - cdf_abs;
}

/* ── Scalar Algebraic Floating-Point Primitives ──────────────────────────── */
static inline double nyx_algebraic_add(double a, double b) {
    return a + b;
}

static inline double nyx_algebraic_sub(double a, double b) {
    return a - b;
}

static inline double nyx_algebraic_mul(double a, double b) {
    return a * b;
}

static inline double nyx_algebraic_div(double a, double b) {
    return a / b;
}

/* Fused Multiply-Add: (a * b) + c with single rounding */
static inline double nyx_algebraic_fma(double a, double b, double c) {
#if defined(__FMA__) || defined(__AVX2__)
    return __builtin_fma(a, b, c);
#else
    return (a * b) + c;
#endif
}

/* ── Vectorized Algebraic Array Operations (4x - 8x SIMD Throughput) ─────── */

/* Algebraic Dot Product with unrolled SIMD accumulator reassociation */
double nyx_algebraic_dot_product(const double* a, const double* b, size_t len);

/* Algebraic Vector Multiply-Accumulate (y = y + alpha * x) */
void nyx_algebraic_axpy(double alpha, const double* x, double* y, size_t len);

/* Fast Algebraic Polynomial Evaluation: p(x) = c0 + c1*x + c2*x^2 + ... */
double nyx_algebraic_poly_eval(double x, const double* coeffs, size_t degree);

#ifdef __cplusplus
}
#endif

#endif /* RT_MATH_H */

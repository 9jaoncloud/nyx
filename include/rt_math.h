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

#ifdef __cplusplus
extern "C" {
#endif

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

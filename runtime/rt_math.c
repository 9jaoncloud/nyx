/*
 * rt_math.c — Nyx Runtime Fast Algebraic Floating-Point Implementation
 */

#include "rt_math.h"
#include <stdio.h>
#include <math.h>

/*
 * Algebraic Dot Product:
 * Allows the compiler to break strict left-to-right IEEE-754 association
 * and use 4-way parallel SIMD accumulators, achieving maximum FMA throughput.
 */
double nyx_algebraic_dot_product(const double* a, const double* b, size_t len) {
    if (!a || !b || len == 0) return 0.0;

    double sum0 = 0.0;
    double sum1 = 0.0;
    double sum2 = 0.0;
    double sum3 = 0.0;

    size_t i = 0;
    size_t unrolled_len = len - (len % 4);

    for (; i < unrolled_len; i += 4) {
        sum0 = nyx_algebraic_fma(a[i + 0], b[i + 0], sum0);
        sum1 = nyx_algebraic_fma(a[i + 1], b[i + 1], sum1);
        sum2 = nyx_algebraic_fma(a[i + 2], b[i + 2], sum2);
        sum3 = nyx_algebraic_fma(a[i + 3], b[i + 3], sum3);
    }

    double total = (sum0 + sum1) + (sum2 + sum3);

    for (; i < len; i++) {
        total = nyx_algebraic_fma(a[i], b[i], total);
    }

    return total;
}

void nyx_algebraic_axpy(double alpha, const double* x, double* y, size_t len) {
    if (!x || !y || len == 0) return;

    size_t i = 0;
    size_t unrolled_len = len - (len % 4);

    for (; i < unrolled_len; i += 4) {
        y[i + 0] = nyx_algebraic_fma(alpha, x[i + 0], y[i + 0]);
        y[i + 1] = nyx_algebraic_fma(alpha, x[i + 1], y[i + 1]);
        y[i + 2] = nyx_algebraic_fma(alpha, x[i + 2], y[i + 2]);
        y[i + 3] = nyx_algebraic_fma(alpha, x[i + 3], y[i + 3]);
    }

    for (; i < len; i++) {
        y[i] = nyx_algebraic_fma(alpha, x[i], y[i]);
    }
}

/* Horner's Scheme with FMA for polynomial evaluation */
double nyx_algebraic_poly_eval(double x, const double* coeffs, size_t degree) {
    if (!coeffs || degree == 0) return 0.0;
    double result = coeffs[degree];
    for (int64_t i = (int64_t)degree - 1; i >= 0; i--) {
        result = nyx_algebraic_fma(result, x, coeffs[i]);
    }
    return result;
}

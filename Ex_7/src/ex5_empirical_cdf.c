#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lcg.h"

#define N_SAMPLES  50000
#define LAMBDA     1.5
#define N_OUTPUT   2000

static int cmp_double(const void* a, const void* b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da > db) - (da < db);
}

int main(void) {
    /* Same seed and generation as ex4 so samples are identical */
    uint32_t state = 246813579u;

    double* y = malloc(N_SAMPLES * sizeof(double));
    if (!y) { fprintf(stderr, "malloc failed\n"); return 1; }

    for (int i = 0; i < N_SAMPLES; i++) {
        double u = lcg_next_double(&state);
        y[i] = -log(1.0 - u) / LAMBDA;
    }

    qsort(y, N_SAMPLES, sizeof(double), cmp_double);

    FILE* f = fopen("output/empirical_cdf.csv", "w");
    if (!f) { fprintf(stderr, "Cannot open output/empirical_cdf.csv\n"); free(y); return 1; }
    fprintf(f, "y,empirical_cdf,exact_cdf\n");

    for (int k = 0; k < N_OUTPUT; k++) {
        int    idx   = (int)((long)k * N_SAMPLES / N_OUTPUT);
        double yv    = y[idx];
        double ecdf  = (double)(idx + 1) / N_SAMPLES;
        double exact = 1.0 - exp(-LAMBDA * yv);
        fprintf(f, "%.8f,%.8f,%.8f\n", yv, ecdf, exact);
    }
    fclose(f);
    free(y);

    printf("Exercise 5 — Empirical CDF vs exact CDF (lambda=%.1f, %d samples)\n",
           LAMBDA, N_SAMPLES);
    printf("  Data written to output/empirical_cdf.csv\n\n");
    return 0;
}

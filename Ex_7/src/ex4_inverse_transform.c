#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lcg.h"

#define N_SAMPLES 200000
#define LAMBDA    1.5
#define N_BINS    80

int main(void) {
    uint32_t state = 246813579u;

    double* y = malloc(N_SAMPLES * sizeof(double));
    if (!y) { fprintf(stderr, "malloc failed\n"); return 1; }

    for (int i = 0; i < N_SAMPLES; i++) {
        double u = lcg_next_double(&state);   /* u in [0,1): log(1-u) is safe */
        y[i] = -log(1.0 - u) / LAMBDA;
    }

    /* Histogram over [0, y_cap]; cap at ~99th percentile */
    double y_cap     = -log(0.01) / LAMBDA;
    double bin_width = y_cap / N_BINS;

    int bins[N_BINS];
    for (int b = 0; b < N_BINS; b++) bins[b] = 0;
    for (int i = 0; i < N_SAMPLES; i++) {
        int b = (int)(y[i] / bin_width);
        if (b >= 0 && b < N_BINS) bins[b]++;
    }

    FILE* f = fopen("output/exponential_pdf.csv", "w");
    if (!f) { fprintf(stderr, "Cannot open output/exponential_pdf.csv\n"); free(y); return 1; }
    fprintf(f, "y_mid,empirical_density,analytic_pdf\n");

    for (int b = 0; b < N_BINS; b++) {
        double y_mid     = (b + 0.5) * bin_width;
        double empirical = (double)bins[b] / (N_SAMPLES * bin_width);
        double analytic  = LAMBDA * exp(-LAMBDA * y_mid);
        fprintf(f, "%.8f,%.8f,%.8f\n", y_mid, empirical, analytic);
    }
    fclose(f);
    free(y);

    printf("Exercise 4 — Inverse transform exponential (lambda=%.1f, %d samples)\n",
           LAMBDA, N_SAMPLES);
    printf("  Data written to output/exponential_pdf.csv\n\n");
    return 0;
}

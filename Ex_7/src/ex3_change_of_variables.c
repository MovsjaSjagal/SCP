#include <stdio.h>
#include <math.h>
#include "lcg.h"

#define N_SAMPLES 200000
#define N_BINS    80

int main(void) {
    uint32_t state = 13579u;

    int bins[N_BINS] = {0};
    double bin_width = 1.0 / N_BINS;

    for (int i = 0; i < N_SAMPLES; i++) {
        double u = lcg_next_double(&state);
        double y = u * u;
        int b = (int)(y / bin_width);
        if (b >= N_BINS) b = N_BINS - 1;
        bins[b]++;
    }

    FILE* f = fopen("output/change_of_variables.csv", "w");
    if (!f) { fprintf(stderr, "Cannot open output/change_of_variables.csv\n"); return 1; }
    fprintf(f, "y_mid,empirical_density,analytic_density\n");

    for (int b = 0; b < N_BINS; b++) {
        double y_mid     = (b + 0.5) * bin_width;
        double empirical = (double)bins[b] / (N_SAMPLES * bin_width);
        double analytic  = 0.5 / sqrt(y_mid);   /* f_Y(y) = 1/(2*sqrt(y)) */
        fprintf(f, "%.8f,%.8f,%.8f\n", y_mid, empirical, analytic);
    }
    fclose(f);

    printf("Exercise 3 — Change of variables Y=U^2 (%d samples)\n", N_SAMPLES);
    printf("  Data written to output/change_of_variables.csv\n\n");
    return 0;
}

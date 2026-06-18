#include <stdio.h>
#include <math.h>
#include "lcg.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const long SIZES[] = { 100, 1000, 10000, 100000, 1000000 };
#define N_SIZES 5

int main(void) {
    /* Two independent LCG streams, matching the notebook's C solution */
    uint32_t state_x = 123u;
    uint32_t state_y = 987654321u;

    FILE* f = fopen("output/monte_carlo_pi.csv", "w");
    if (!f) { fprintf(stderr, "Cannot open output/monte_carlo_pi.csv\n"); return 1; }
    fprintf(f, "N,pi_estimate,error\n");

    printf("Exercise 2 — Monte Carlo estimate of pi\n");
    printf("  %-10s  %-14s  %-12s\n", "N", "pi_estimate", "error");
    printf("  %-10s  %-14s  %-12s\n", "----------", "--------------", "------------");

    long inside = 0;
    int  si = 0;

    for (long i = 1; i <= SIZES[N_SIZES - 1]; i++) {
        double x = lcg_next_double(&state_x);
        double y = lcg_next_double(&state_y);
        if (x * x + y * y <= 1.0) inside++;

        if (si < N_SIZES && i == SIZES[si]) {
            double pi_est = 4.0 * inside / i;
            double err    = fabs(pi_est - M_PI);
            printf("  %-10ld  %-14.8f  %.8f\n", i, pi_est, err);
            fprintf(f, "%ld,%.10f,%.10f\n", i, pi_est, err);
            si++;
        }
    }
    printf("\n");
    fclose(f);
    return 0;
}

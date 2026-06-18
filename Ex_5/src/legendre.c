/*
 * legendre.c — Homework 05
 *
 * Computes Legendre polynomials P_l(x) for l = 0..L_MAX and
 * x in {0.1, 0.5, 0.9, 0.99} using two methods:
 *
 *   1) Forward recurrence (Bonnet):
 *        P_0 = 1,  P_1 = x
 *        P_{l+1} = ((2l+1)*x*P_l - l*P_{l-1}) / (l+1)
 *
 *   2) Backward (Miller-style):
 *        Start with P~_{L+1} = 0, P~_L = 1
 *        P~_{l-1} = ((2l+1)/l)*x*P~_l - ((l+1)/l)*P~_{l+1}
 *        Rescale: c = 1/P~_0,  P_l^back = c * P~_l
 *
 * Output: output/legendre_recurrence.csv
 * Columns: x, l, P_forward, P_backward
 */

#include <stdio.h>
#include <stdlib.h>

#define L_MAX 50

static void forward_recurrence(double x, double* P) {
    P[0] = 1.0;
    P[1] = x;
    for (int l = 1; l < L_MAX; l++)
        P[l + 1] = ((2.0 * l + 1.0) * x * P[l] - l * P[l - 1]) / (l + 1.0);
}

static void backward_recurrence(double x, double* P) {
    /* Allocate one extra slot for P~_{L_MAX+1} */
    double Pt[L_MAX + 2];

    Pt[L_MAX + 1] = 0.0;
    Pt[L_MAX]     = 1.0;

    for (int l = L_MAX; l >= 1; l--)
        Pt[l - 1] = ((2.0 * l + 1.0) / l) * x * Pt[l]
                  - ((double)(l + 1) / l) * Pt[l + 1];

    /* Rescale so that P~_0 == 1  (since P_0(x) = 1 exactly) */
    double c = 1.0 / Pt[0];
    for (int l = 0; l <= L_MAX; l++)
        P[l] = c * Pt[l];
}

int main(void) {
    double x_vals[] = { 0.1, 0.5, 0.9, 0.99 };
    int n_x = 4;

    double P_fwd[L_MAX + 1];
    double P_bwd[L_MAX + 1];

    FILE* f = fopen("output/legendre_recurrence.csv", "w");
    if (!f) { fprintf(stderr, "Cannot open output/legendre_recurrence.csv\n"); return 1; }
    fprintf(f, "x,l,P_forward,P_backward\n");

    for (int xi = 0; xi < n_x; xi++) {
        double x = x_vals[xi];
        forward_recurrence(x, P_fwd);
        backward_recurrence(x, P_bwd);
        for (int l = 0; l <= L_MAX; l++)
            fprintf(f, "%.2f,%d,%.17e,%.17e\n", x, l, P_fwd[l], P_bwd[l]);
    }

    fclose(f);
    printf("Legendre recurrences written to output/legendre_recurrence.csv\n");
    return 0;
}

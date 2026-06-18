#include <stdio.h>
#include "lcg.h"

#define N_TOSSES 100000

int main(void) {
    uint32_t state = 123456789u;
    int heads = 0;

    FILE* f = fopen("output/coin_toss.csv", "w");
    if (!f) { fprintf(stderr, "Cannot open output/coin_toss.csv\n"); return 1; }
    fprintf(f, "n,fraction_heads\n");

    for (int i = 1; i <= N_TOSSES; i++) {
        heads += (lcg_next_double(&state) < 0.5) ? 1 : 0;

        /* Record first 50 tosses individually, then every 500 */
        if (i <= 50 || i % 500 == 0)
            fprintf(f, "%d,%.8f\n", i, (double)heads / i);
    }
    fclose(f);

    printf("Exercise 1 — Coin toss LLN (%d tosses)\n", N_TOSSES);
    printf("  Final fraction of heads: %.6f  (expected 0.5)\n\n",
           (double)heads / N_TOSSES);
    return 0;
}

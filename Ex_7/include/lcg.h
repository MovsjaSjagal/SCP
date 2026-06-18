#ifndef LCG_H
#define LCG_H

#include <stdint.h>

/*
 * 32-bit Linear Congruential Generator.
 *
 * Recurrence:  x_{n+1} = a * x_n + c  (mod 2^32)
 * Parameters:  a = 1664525, c = 1013904223  (Numerical Recipes)
 *
 * lcg_next_double maps the 32-bit state to [0, 1) by dividing by 2^32.
 * The open upper bound means log(1-u) is always safe.
 */

static inline uint32_t lcg_next(uint32_t *state) {
    *state = 1664525u * (*state) + 1013904223u;
    return *state;
}

static inline double lcg_next_double(uint32_t *state) {
    return lcg_next(state) / 4294967296.0;
}

#endif /* LCG_H */

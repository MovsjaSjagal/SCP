#include "daxpy.h"

#include <math.h>

void daxpy_reference(double* d, double a, const double* x, const double* y, int n) {
    for (int i = 0; i < n; i++)
        d[i] = a * x[i] + y[i];
}

int daxpy_chunked(double* d, double a, const double* x, const double* y,
    int n, int chunk_size, double* partial_sums) {
    int num_chunks = (int)ceil((double)n / chunk_size);

    for (int ci = 0; ci < num_chunks; ci++) {
        int start = ci * chunk_size;
        int end = start + chunk_size;
        if (end > n) end = n;

        /* compute d for this chunk */
        for (int i = start; i < end; i++)
            d[i] = a * x[i] + y[i];

        /* partial sum for this chunk */
        partial_sums[ci] = 0.0;
        for (int i = start; i < end; i++)
            partial_sums[ci] += d[i];
    }

    return num_chunks;
}


double vector_sum(const double* v, int n) {
    double s = 0.0;
    for (int i = 0; i < n; i++) s += v[i];
    return s;
}

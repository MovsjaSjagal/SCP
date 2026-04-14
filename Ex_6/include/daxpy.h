#ifndef DAXPY_H
#define DAXPY_H

/*
 * daxpy_reference
 *   Computes d[i] = a*x[i] + y[i] for i in [0, n) in a single loop.
 *   d must be pre-allocated with n elements.
 */
void daxpy_reference(double* d, double a, const double* x, const double* y, int n);

/*
 * daxpy_chunked
 *   Same computation as daxpy_reference, split into chunks of chunk_size.
 *   d and partial_sums must be pre-allocated.
 *   partial_sums must have ceil(n / chunk_size) elements.
 *   Returns the number of chunks used.
 */
int daxpy_chunked(double* d, double a, const double* x, const double* y,
    int n, int chunk_size, double* partial_sums);

/*
 * vector_sum
 *   Returns the sum of elements in v[0..n-1].
 */
double vector_sum(const double* v, int n);


#endif /* DAXPY_H */

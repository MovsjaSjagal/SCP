#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "config.h"
#include "daxpy.h"
#include "writehdf5.h"

#define CONFIG_PATH "config.ini"
#define EPSILON     1e-10

int main(void) {

    /* === DEFINE VARIABLES === */
    /* --- load configuration --- */
    Config cfg;
    config_load(&cfg, CONFIG_PATH);

    int    n = config_get_int(&cfg, "n");
    int    chunk_size = config_get_int(&cfg, "chunk_size");
    double a = config_get_double(&cfg, "a");
    double x_val = config_get_double(&cfg, "x_val");
    double y_val = config_get_double(&cfg, "y_val");

    printf("Config: n=%d  chunk_size=%d  a=%.4f  x_val=%.4f  y_val=%.4f\n\n",
        n, chunk_size, a, x_val, y_val);


    /* === DEFINE VECTORS AND COMPUTE DAXPY === */

    /* --- allocate memory --- */
    double* x = malloc(n * sizeof(double));
    double* y = malloc(n * sizeof(double));
    double* d_ref = malloc(n * sizeof(double));
    double* d_chunked = malloc(n * sizeof(double));
    int     num_chunks = (int)ceil((double)n / chunk_size);
    double* partial_sums = malloc(num_chunks * sizeof(double));

    if (!x || !y || !d_ref || !d_chunked || !partial_sums) {
        fprintf(stderr, "malloc failed\n");
        return EXIT_FAILURE;
    }
    
    /* fill input vectors */
    for (int i = 0; i < n; i++) { x[i] = x_val; y[i] = y_val; }

    /* --- reference daxpy --- */
    daxpy_reference(d_ref, a, x, y, n);
    double ref_sum = vector_sum(d_ref, n);

    /* --- chunked daxpy --- */
    daxpy_chunked(d_chunked, a, x, y, n, chunk_size, partial_sums);

    /* sum of partial sums */
    double chunked_sum = vector_sum(partial_sums, num_chunks);


    /* === CHECKS === */

    /* --- check 1: element-wise equality --- */
    int mismatch = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(d_chunked[i] - d_ref[i]) > EPSILON) { mismatch = 1; break; }
    }
    printf("Check 1: d_chunked         == d_ref:      %s\n", mismatch ? "FAIL" : "PASS");

    /* --- check 2: total sums agree --- */
    int sums_match = fabs(chunked_sum - ref_sum) < EPSILON;
    printf("Check 2: sum(partial_sums) == sum(d_ref): %s\n", sums_match ? "PASS" : "FAIL");
    printf("  ref_sum      = %.6f\n", ref_sum);
    printf("  chunked_sum  = %.6f\n", chunked_sum);

    /* === WRITE INTO HDF5 FILES === */

    printf("\nWriting HDF5 file...\n");
    write_results_hdf5("results.h5", d_ref, d_chunked, partial_sums, n, num_chunks, chunk_size, a);
    printf("\nHDF5 file complete.\n");

    /* === FINISHING TASKS === */

    /* --- cleanup --- */
    free(x);
    free(y);
    free(d_ref);
    free(d_chunked);
    free(partial_sums);

    return EXIT_SUCCESS;
}

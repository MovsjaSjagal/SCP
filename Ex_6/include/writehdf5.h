#ifndef WRITEHDF5_H
#define WRITEHDF5_H

#include <hdf5.h>
#include "hdf5.h"

int write_results_hdf5(
    const char* filename,
    const double* d_ref,
    const double* d_chunked,
    const double* partial_sums,
    int n,
    int num_chunks,
    int chunk_size,
    double a);






#endif /* WRITEHDF5_H */

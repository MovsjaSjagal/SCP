#include "writehdf5.h"

#include "hdf5.h"
#include <hdf5.h>
#include <stdio.h>


int write_results_hdf5(const char* filename,
    const double* d_ref,
    const double* d_chunked,
    const double* partial_sums,
    int n,
    int num_chunks,
    int chunk_size,
    double a)
{
    /* === Create file === */
    hid_t file = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file < 0) return -1;

    /* === Create dataspace for vectors === */
    hsize_t n_dims[1] = { (hsize_t)n };
    hsize_t c_dims[1] = { (hsize_t)num_chunks };

    hid_t space_n = H5Screate_simple(1, n_dims, NULL);
    hid_t space_c = H5Screate_simple(1, c_dims, NULL);

    /* === Create datasets === */
    hid_t dset_ref = H5Dcreate(file, "/d_ref", H5T_NATIVE_DOUBLE, space_n,
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t dset_chunked = H5Dcreate(file, "/d_chunked", H5T_NATIVE_DOUBLE, space_n,
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t dset_partial = H5Dcreate(file, "/partial_sums", H5T_NATIVE_DOUBLE, space_c,
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    if (dset_ref < 0 || dset_chunked < 0 || dset_partial < 0)
        return -2;

    /* === Write data === */
    H5Dwrite(dset_ref, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, d_ref);
    H5Dwrite(dset_chunked, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, d_chunked);
    H5Dwrite(dset_partial, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, partial_sums);

    /* === Create metadata group === */
    hid_t meta = H5Gcreate(file, "/meta", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hid_t attr_space = H5Screate(H5S_SCALAR);

    hid_t attr_n = H5Acreate(meta, "n", H5T_NATIVE_INT, attr_space,
        H5P_DEFAULT, H5P_DEFAULT);
    H5Awrite(attr_n, H5T_NATIVE_INT, &n);

    hid_t attr_chunk = H5Acreate(meta, "chunk_size", H5T_NATIVE_INT, attr_space,
        H5P_DEFAULT, H5P_DEFAULT);
    H5Awrite(attr_chunk, H5T_NATIVE_INT, &chunk_size);

    hid_t attr_a = H5Acreate(meta, "a", H5T_NATIVE_DOUBLE, attr_space,
        H5P_DEFAULT, H5P_DEFAULT);
    H5Awrite(attr_a, H5T_NATIVE_DOUBLE, &a);

    /* === Cleanup === */
    H5Aclose(attr_n);
    H5Aclose(attr_chunk);
    H5Aclose(attr_a);

    H5Sclose(attr_space);
    H5Gclose(meta);

    H5Dclose(dset_ref);
    H5Dclose(dset_chunked);
    H5Dclose(dset_partial);

    H5Sclose(space_n);
    H5Sclose(space_c);

    H5Fclose(file);

    return 0;
}

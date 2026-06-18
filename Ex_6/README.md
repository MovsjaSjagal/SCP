# Homework 06 — Chunked DAXPY with HDF5 Output

## What this program does

Computes the DAXPY operation `d[i] = a*x[i] + y[i]` in two ways — a plain
single loop (reference) and a chunk-based loop — then verifies both produce
identical results and writes everything to an HDF5 file.

The exercise has four distinct goals:

1. Implement chunked iteration and verify it matches the reference.
2. Compute partial sums per chunk and verify they sum to the same total.
3. Read all runtime parameters from a config file instead of hardcoding them.
4. Persist the results in HDF5 format.

---

## File structure

```
src/
  main.c          orchestrates config loading, computation, checks, and output
  daxpy.c         daxpy_reference, daxpy_chunked, vector_sum
  config.c        lightweight key=value config-file parser
  writehdf5.c     HDF5 output
include/
  daxpy.h
  config.h
  writehdf5.h
config.ini        runtime parameters
Makefile
```

---

## Dependencies

- HDF5 (`h5cc` compiler wrapper, provided by `hdf5-devel` / `libhdf5-dev`)
- libm (`-lm`, for `ceil()`)

The Makefile uses `h5cc`, which is HDF5's compiler wrapper. It automatically
adds the correct include paths and linker flags for the HDF5 library, so no
manual `-I` or `-L` flags for HDF5 are needed.

---

## Build and run

```bash
make
./exercise_6
```

Output is printed to stdout and written to `results.h5`.

---

## Configuration — `config.ini`

All parameters are read at runtime from `config.ini`. This avoids recompiling
when changing input values, which is standard practice for scientific codes.

Format: `key = value`, one per line. Lines starting with `#` are treated as
comments and ignored.

| Key          | Type   | Description                       |
|--------------|--------|-----------------------------------|
| `n`          | int    | Total vector length               |
| `chunk_size` | int    | Number of elements per chunk      |
| `a`          | double | Scalar multiplier                 |
| `x_val`      | double | Uniform fill value for vector `x` |
| `y_val`      | double | Uniform fill value for vector `y` |

The parser (`config.c`) is a minimal hand-rolled implementation: it reads
lines, splits on `=`, trims whitespace, and stores key-value pairs in a
fixed-size struct array. It was written from scratch rather than pulling in an
external library, as the required feature set is small.

---

## Chunked iteration — why and how

### Why chunks?

Splitting work into fixed-size chunks is the foundation of many parallel and
cache-aware algorithms. In a multithreaded or SIMD context, each chunk would be
assigned to a separate thread or vector lane. Here the goal is to understand
the bookkeeping before adding parallelism.

### Index arithmetic

Given `n` total elements and `chunk_size` elements per chunk:

```
num_chunks = ceil(n / chunk_size)
```

Using `ceil` ensures the last partial chunk (when `n` is not a multiple of
`chunk_size`) is not silently dropped. For chunk index `ci`:

```
start = ci * chunk_size
end   = min(start + chunk_size, n)   // clamp at n for the last chunk
```

All indices are 0-based throughout (`start` inclusive, `end` exclusive),
consistent with standard C array indexing.

Example — n=100, chunk_size=8:
- Chunks 0–11: elements 0–7, 8–15, …, 88–95 (8 elements each)
- Chunk 12: elements 96–99 (4 elements, last chunk is smaller)
- `num_chunks = ceil(100/8) = 13`

### Partial sums

After computing `d[i]` for each chunk, the chunk's contribution to the total
sum is accumulated into `partial_sums[ci]`. Summing `partial_sums` then gives
the same result as summing all of `d` directly. This is the basis of parallel
reduction.

---

## Correctness checks

Two checks are performed before writing output:

**Check 1 — element-wise equality:**
Every element of `d_chunked` is compared to `d_ref` within a tolerance of
`1e-10`. Since both perform identical floating-point operations in the same
order on the same data, the results are bit-for-bit identical and the tolerance
is never needed in practice — but it is good habit to use `fabs` + epsilon
rather than `==` for floating-point comparisons.

**Check 2 — sum agreement:**
`sum(partial_sums)` is compared to `sum(d_ref)`. This verifies that the
per-chunk accumulation is correct. Both checks print `PASS` or `FAIL`.

---

## HDF5 output — `results.h5`

HDF5 (Hierarchical Data Format 5) is the standard binary format for scientific
data. It stores typed, self-describing datasets inside a file that behaves like
a directory tree. Unlike plain `.txt` or `.csv`, it supports:

- Exact binary representation (no floating-point text conversion loss)
- Metadata stored alongside data (attributes)
- Efficient I/O for large arrays
- Direct compatibility with Python (`h5py`), MATLAB, and Julia

### File layout

```
/d_ref          (double[n])      — result from the reference single loop
/d_chunked      (double[n])      — result from the chunked loop
/partial_sums   (double[num_chunks]) — sum of d[i] within each chunk
/meta/          (group)
    n           (int attribute)
    chunk_size  (int attribute)
    a           (double attribute)
```

### HDF5 API pattern used

Each dataset follows the same three-step HDF5 write pattern:

```c
hid_t space = H5Screate_simple(1, dims, NULL);   // define shape
hid_t dset  = H5Dcreate(file, "/name", ...);     // create dataset in file
H5Dwrite(dset, H5T_NATIVE_DOUBLE, ...);          // write data
H5Dclose(dset);
H5Sclose(space);
```

Attributes (scalar metadata) follow the same idea with `H5Acreate` /
`H5Awrite` / `H5Aclose`.

### Inspecting the output

```bash
h5dump -n results.h5          # list file contents
h5dump -d /partial_sums results.h5   # print partial_sums dataset
```

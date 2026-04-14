# Homework 06 — Chunked DAXPY

Computes `d = a*x + y` (DAXPY) in two ways — a single reference loop and a
chunk-based loop — then verifies they agree and writes the results to an HDF5
file.

## File Structure

```
main.c          entry point
daxpy.h/c       daxpy_reference, daxpy_chunked, vector_sum
config.h/c      lightweight key=value config-file parser
writehdf5.h/c   HDF5 output (writes results.h5)
config.ini      runtime parameters
Makefile
```

## Dependencies

- GCC (C11)
- libm (`-lm`)
- HDF5 (`libhdf5-dev` on Debian/Ubuntu)

## Build

```bash
make
```

## Run

```bash
./exercise_6
```

Output is printed to stdout and written to `results.h5`.

## Configuration — `config.ini`

Parameters are read at runtime from `config.ini`. Format: `key = value`, one per line. Lines starting with `#` are ignored.

| Key          | Type   | Description                        |
|--------------|--------|------------------------------------|
| `n`          | int    | Vector length                      |
| `chunk_size` | int    | Elements per chunk                 |
| `a`          | double | Scalar multiplier                  |
| `x_val`      | double | Uniform fill value for vector `x`  |
| `y_val`      | double | Uniform fill value for vector `y`  |

Example:

```ini
n          = 100
chunk_size = 8
a          = 2.0
x_val      = 3.0
y_val      = 1.0
```

## Checks

The program performs two correctness checks before writing output:

1. **Element-wise** — every element of `d_chunked` matches `d_ref` within `1e-10`.
2. **Sum** — the sum of `partial_sums` matches `sum(d_ref)` within `1e-10`.

Both print `PASS` or `FAIL` to stdout.

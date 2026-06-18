# Homework 03 — Matrix Multiplication & Cache Performance

## What this program does

Multiplies two N×N diagonal matrices A = aI and B = bI, producing C = AB = abI.
Three strategies are benchmarked and their results cross-validated before the
output matrix is written to disk.

---

## Usage

```bash
make
./program <a> <b> <N> <fileout>
```

Example:
```bash
./program 2.0 3.0 1000 result
```

Defaults (used when no arguments are provided): `a=2.0`, `b=3.0`, `N=1000`, `fileout="result"`.

Output files produced: `result.txt`, `result.csv`, `result.bin`.

---

## Project structure

```
src/
  main.c        — orchestrates input, multiplication, validation, and file output
  matrix_io.c   — all implementation: creation, multiplication, validation, I/O
  matrix_io.h   — declarations and per-function documentation
Makefile
```

---

## Why diagonal matrices?

The assignment asks to fill A with a constant `a` and B with a constant `b`. Using
A = aI (the identity scaled by a) and B = bI gives an analytically known result:

```
C = AB = (aI)(bI) = ab * I
```

Every diagonal element of C must equal `a*b`, and every off-diagonal element must
be zero. This makes validation trivial and exact.

---

## Validation: why checking only the diagonal is sufficient

The assignment asks: *is there a quicker way to check without comparing every element?*

Yes. Because the inputs are diagonal, the matrix product formula
`C[i][j] = sum_k A[i][k] * B[k][j]` receives non-zero contributions only when
`i == k` and `k == j`, i.e. only when `i == j`. Off-diagonal elements of C are
guaranteed to be zero by the structure of the inputs, so only the N diagonal
elements need to be checked:

```c
for (int i = 0; i < N; i++)
    if (fabs(matrix[i][i] - a*b) > 1e-12) return 0;
```

This reduces validation from O(N²) to O(N).

---

## Memory layout: 2D pointer array vs flat 1D array

Two representations of the same logical N×N matrix are used:

**`double**` (pointer-to-pointer, 2D):**
```
matrix[0] → [row 0 data | somewhere in heap]
matrix[1] → [row 1 data | somewhere else in heap]
...
```
Each row is a separate `malloc` call. The rows are *not* guaranteed to be
contiguous in memory. Accessing `matrix[i][j]` requires two pointer dereferences:
one to find the row pointer, one to reach the element.

**`double*` (flat 1D, row-major):**
```
[ row0_col0, row0_col1, ..., row0_colN-1, row1_col0, row1_col1, ... ]
```
The entire matrix lives in one contiguous block. Element (i,j) is at index
`i*N + j`. One pointer dereference, and all elements are addressable by
simple arithmetic.

The flat layout is faster in practice because the CPU prefetcher can predict
sequential memory access patterns and load upcoming data into cache ahead of
time. With `double**`, the prefetcher cannot follow the pointer chain to the
next row without first loading the row pointer itself.

---

## Cache locality: why loop order matters

Modern CPUs load memory in **cache lines** of 64 bytes (= 8 doubles) at a time.
When you access one element, the 7 neighbouring elements in memory come along for
free — as long as you access them next (spatial locality). If you jump to a
different cache line before using them, those 7 free loads are wasted, and the
next access triggers a new (slow) load from RAM. This is a **cache miss**.

C stores 2D arrays in **row-major order**: all elements of row `i` are
contiguous in memory, followed immediately by row `i+1`. Traversing a row by
incrementing `j` is cache-friendly; jumping between rows by incrementing `i`
with fixed `j` (column-wise) is not.

### ijk ordering — cache-unfriendly

```c
for (int i = 0; i < N; i++)
  for (int j = 0; j < N; j++)
    for (int k = 0; k < N; k++)
      result[i][j] += matrix1[i][k] * matrix2[k][j];
```

In the innermost loop, `k` increments:
- `matrix1[i][k]`: k moves along row i → **cache hit** (row-major, sequential)
- `matrix2[k][j]`: k moves down column j → **cache miss every step** (jumps
  64+ bytes between rows for any N larger than 8)

For N=1000, `matrix2` has rows 1000 doubles = 8000 bytes apart. Each inner
loop iteration jumps to a completely different cache line in matrix2. The CPU
cache fills up with stale lines and is constantly reloaded from RAM.

### ikj ordering — cache-friendly

```c
for (int i = 0; i < N; i++)
  for (int k = 0; k < N; k++) {
    double r = matrix1[i][k];   // hoisted: read once, reused N times
    for (int j = 0; j < N; j++)
      result[i][j] += r * matrix2[k][j];
  }
```

In the innermost loop, `j` increments:
- `matrix2[k][j]`: j moves along row k → **cache hit**
- `result[i][j]`: j moves along row i → **cache hit**
- `matrix1[i][k]`: hoisted out of the inner loop as scalar `r` → **no load at all**

Every inner loop iteration is a sequential step through two rows already in
cache. This is the optimal access pattern for row-major storage.

### Flat ikj — best of both

`multiplyVector` uses the same ikj order, but on a flat `double*` array.
The index arithmetic `i*N+k`, `k*N+j`, `i*N+j` maps directly to contiguous
memory, and the single allocation eliminates the pointer-chasing overhead of
`double**`. This is the fastest of the three.

### Expected benchmark ranking (fastest to slowest)

| Strategy          | Inner loop access pattern            | Expected speed |
|-------------------|--------------------------------------|----------------|
| flat ikj          | row-wise, single contiguous block    | fastest        |
| 2D ikj            | row-wise, separate row allocations   | middle         |
| 2D ijk            | column-wise on matrix2               | slowest        |

---

## Timing method

All three multiplications are timed with `clock()` from `<time.h>`:

```c
clock_t start = clock();
/* ... computation ... */
clock_t end = clock();
double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
```

`clock()` returns CPU time consumed by the process (not wall time), which
isolates computation cost from OS scheduling noise on lightly loaded systems.

---

## Output formats

| Format | Notes |
|--------|-------|
| `.txt` | Space-separated rows; human-readable, slow for large N |
| `.csv` | Comma-separated; opens in Excel/Python/MATLAB, still text-based |
| `.bin` | Raw binary: `[N: size_t][N: size_t][row data...]`; fast I/O, no precision loss, not portable across architectures |

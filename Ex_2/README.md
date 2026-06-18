# Homework 02 — Linear Vector Operation

## Program Description

The program `exercise_2.c` computes the linear vector combination:

$$\vec{d} = a\vec{x} + \vec{y}$$

where `a` is a scalar and `x`, `y` are vectors of dimension N, with all elements of each vector being equal. The program allows the user to input different values for `a`, `x`, `y`, and `N` at every run via the command line, making it easy to test different configurations without recompiling.

### Compile and Run

```bash
gcc exercise_2.c -o exercise_2
./exercise_2
```

### Tested Dimensions

The program was tested with N = 10, 10^6, and 10^8, all of which ran correctly.

---

## Question 1 — Problems for Large N

For very large values of N, the program encountered memory exhaustion errors, since it allocates three separate arrays of N doubles dynamically using `malloc()`. Each double occupies 8 bytes, so the total memory required for the three arrays is approximately:

$$3 \times N \times 8 \text{ bytes}$$

The following behaviour was observed:

| N | Behaviour |
|---|-----------|
| 10 |  Correct |
| 10^6 |  Correct |
| 10^8 |  Correct |
| 10^9 |  `Killed` — the OS terminates the process when it attempts to allocate ~24 GB of RAM |
| 10^10 |  `Segmentation fault (core dumped)` — `malloc()` returns `NULL` and a null pointer is dereferenced |

The difference in error type between 10^9 and 10^10 is due to how the OS handles the requests. At 10^9, `malloc()` may initially succeed (due to optimistic memory allocation), but the process is killed by the OS out-of-memory manager (OOM killer) when it actually tries to use the memory. At 10^10, the requested allocation is so large that `malloc()` fails immediately and returns `NULL`, leading to a segmentation fault on the subsequent write.

Note: the dimension variable is stored as `long` (64-bit) rather than `int` (32-bit) to correctly handle values above ~2.1 × 10^9 without integer overflow.

---

## Question 2 — Test with a = 3, x = 0.1, y = 7.1

The mathematically expected result is:

$$d = 3 \times 0.1 + 7.1 = 0.3 + 7.1 = 7.4$$

However, this test reveals a classic **floating-point representation problem**. Neither `0.1` nor `7.1` has an exact representation in IEEE 754 binary64 (double precision). Printing the result with `%f` (6 decimal places) gives `7.400000`, which appears correct, but printing with full precision (`%.17f`) exposes the error:

```
Result (6 decimal places):  7.400000
Result (full precision):    7.39999999999999947
result[0] == 7.4 exactly?   NO  <- floating-point representation error
```

The exact equality check `result[0] == 7.4` evaluates to `false`. This is expected: the stored value is `7.39999999999999947...`, not the mathematical `7.4`. The discrepancy arises because floating-point numbers are stored in base 2, and decimal fractions like `0.1` and `7.1` require infinitely many binary digits — they are rounded to the nearest representable value at the moment of storage.

This is a fundamental property of floating-point arithmetic. When checking results in numerical computing, exact equality (`==`) should generally be replaced by a tolerance-based comparison, e.g.:

```c
if (fabs(result[i] - expected) < 1e-10) { /* close enough */ }
```

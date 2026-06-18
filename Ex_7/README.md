# Homework 07 — Monte Carlo & Random Variable Generation

Five self-contained programs exploring the Law of Large Numbers, Monte Carlo
integration, and classical random variable generation techniques using C's
standard `rand()`.

---

## Build and run

```bash
make run     # build all five programs and execute them
make plot    # also generate PNG plots via gnuplot (requires gnuplot)
make clean   # remove binaries and output/
```

All CSV output lands in `output/`, all plots in `output/*.png`.

---

## File structure

```
src/
  ex1_coin_toss.c           LLN demonstration with a fair coin
  ex2_monte_carlo_pi.c      Monte Carlo estimate of π
  ex3_change_of_variables.c Y = U² transformation
  ex4_inverse_transform.c   Exponential via inverse CDF
  ex5_empirical_cdf.c       Empirical vs exact CDF
include/
  lcg.h                     LCG implementation (shared by all exercises)
plots/
  plot_all.gp               Gnuplot script for all five exercises
Makefile
```

---

## RNG: the notebook LCG

All programs use the 32-bit LCG introduced in the tutorial notebook:

$$x_{n+1} = 1664525 \cdot x_n + 1013904223 \pmod{2^{32}}$$

implemented in `include/lcg.h`. Dividing by 2³² maps the integer state to
`[0, 1)` (open upper bound), so `log(1 - u)` is always safe without any
special-casing.

The LCG is a simple, pedagogically transparent generator. For production Monte
Carlo you would use a higher-quality generator such as Mersenne Twister or PCG.

Exercise 2 uses **two independent LCG streams** (separate states `123u` and
`987654321u`) for the x and y coordinates, matching the notebook's C solution.

---

## Exercise 1 — Coin toss and the Law of Large Numbers

**File:** `ex1_coin_toss.c` | **Output:** `output/coin_toss.csv`

Simulates `N = 100,000` fair coin tosses and records the running fraction of
heads after each toss.

The **Law of Large Numbers** states that for i.i.d. Bernoulli(0.5) variables:

$$\bar{X}_n = \frac{1}{n}\sum_{i=1}^{n} X_i \xrightarrow{a.s.} 0.5 \quad \text{as } n \to \infty$$

In the plot (log x-axis), the fraction starts far from 0.5 for small n and
converges as the number of tosses grows. The fluctuations shrink roughly as
`1/√n` (Central Limit Theorem), visible as the oscillations narrowing around
the 0.5 reference line.

---

## Exercise 2 — Monte Carlo estimate of π

**File:** `ex2_monte_carlo_pi.c` | **Output:** `output/monte_carlo_pi.csv`

Uniformly samples points `(x, y)` in the unit square `[0,1]²` and checks
whether they fall inside the quarter unit disk `x² + y² ≤ 1`. Since the
area of the quarter disk is π/4 and the area of the square is 1:

$$\hat{\pi}_N = 4 \cdot \frac{\text{points inside circle}}{N}$$

**Error convergence:** The standard deviation of the MC estimator decreases as
`σ/√N`, where `σ² = π(4 − π) ≈ 2.70`. On a log-log plot the error follows a
straight line with slope −½, confirming the expected `~1/√N` rate.

| N | π estimate | error |
|---|---|---|
| 100 | 3.08000000 | 0.061 |
| 1,000 | 3.18400000 | 0.042 |
| 10,000 | 3.13320000 | 0.008 |
| 100,000 | 3.13740000 | 0.004 |
| 1,000,000 | 3.13878400 | 0.003 |

The convergence is slow by design: halving the error requires quadrupling the
sample size. This is the fundamental cost of basic Monte Carlo.

---

## Exercise 3 — Change of variables: Y = U²

**File:** `ex3_change_of_variables.c` | **Output:** `output/change_of_variables.csv`

Generates `U ~ Uniform(0,1)` and sets `Y = U²`. By the change-of-variables
formula, the density of `Y` is derived from the density of `U`:

$$f_Y(y) = f_U(u)\left|\frac{du}{dy}\right| = 1 \cdot \frac{1}{2\sqrt{y}} = \frac{1}{2\sqrt{y}}, \quad y \in [0,1]$$

This density is **singular at y = 0** (integrable, but unbounded), which is
visible in both the histogram and the analytic curve as the density rises
steeply near zero. The empirical histogram is normalized to density (divided by
`N * bin_width`) so it can be compared directly with the analytic curve.

---

## Exercise 4 — Inverse transform: Exponential distribution

**File:** `ex4_inverse_transform.c` | **Output:** `output/exponential_pdf.csv`

The **inverse transform method** generates samples from any distribution whose
CDF is analytically invertible. For `Y ~ Exponential(λ)`:

$$F_Y(y) = 1 - e^{-\lambda y} \implies F_Y^{-1}(u) = -\frac{\ln(1-u)}{\lambda}$$

So if `U ~ Uniform(0,1)`, then `Y = -ln(1−U)/λ` is exponentially distributed.

With `λ = 1.5`, the PDF is `f_Y(y) = 1.5 · e^{−1.5y}`. The histogram is
computed over `[0, y_{cap}]` where `y_{cap} = -ln(0.01)/λ ≈ 3.07` covers 99%
of the probability mass; values in the tail beyond this are excluded from
binning to avoid sparsely populated bins distorting the density scale.

---

## Exercise 5 — Empirical CDF

**File:** `ex5_empirical_cdf.c` | **Output:** `output/empirical_cdf.csv`

Uses the same samples as Exercise 4 (same seed and generation sequence). The
**empirical CDF** is defined as:

$$\hat{F}_n(y) = \frac{1}{n}\sum_{i=1}^{n} \mathbf{1}[Y_i \leq y]$$

In practice this is computed by sorting the samples and reading off their rank:
the `k`-th sorted value has empirical CDF `k/n`. The exact CDF for comparison
is `F(y) = 1 − e^{−λy}`.

The Glivenko–Cantelli theorem guarantees `sup_y |F̂_n(y) − F(y)| → 0` almost
surely. With 100,000 samples the empirical CDF is nearly indistinguishable from
the exact one on the plot; the small visible deviations are larger in the tail
where sample density is low.

The sorted array is subsampled to 2,000 output points to keep the CSV
manageable without losing visual resolution.

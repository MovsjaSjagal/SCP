# Homework 05 — Numerical Stability of Legendre Polynomial Recurrences

Compares two ways of computing the Legendre polynomials $P_\ell(x)$ for
$\ell = 0, \dots, 50$ and $x \in \{0.1, 0.5, 0.9, 0.99\}$:

1. **Forward recurrence** (Bonnet's formula, double precision)
2. **Backward recurrence** (Miller-style experiment, double precision)

Both are validated against a high-precision reference computed with `mpmath`
at 50 decimal digits.

---

## Build and run

```bash
make run           # compile C program + generate both CSVs
make plot-python   # also generate all plots
make clean         # remove binary and output/
```

All output lands in `output/`.

---

## File structure

```
src/
  legendre.c         forward and backward recurrences → output/legendre_recurrence.csv
scripts/
  reference.py       mpmath high-precision reference  → output/legendre_reference.csv
  plot_errors.py     loads both CSVs, computes errors, saves plots
Makefile
```

---

## The mathematics

### Forward recurrence (Bonnet's formula)

$$P_0(x) = 1, \qquad P_1(x) = x$$

$$P_{\ell+1}(x) = \frac{(2\ell+1)\,x\,P_\ell(x) - \ell\,P_{\ell-1}(x)}{\ell+1}$$

This is exact in exact arithmetic. The question is whether rounding errors
accumulate as $\ell$ increases.

### Backward (Miller-style) recurrence

Rearranging Bonnet for $P_{\ell-1}$:

$$P_{\ell-1}(x) = \frac{2\ell+1}{\ell}\,x\,P_\ell(x) - \frac{\ell+1}{\ell}\,P_{\ell+1}(x)$$

The Miller experiment starts from arbitrary values at the top and propagates down:

$$\tilde{P}_{L+1} = 0, \qquad \tilde{P}_L = 1$$

then rescales using the known value $P_0(x) = 1$:

$$c = \frac{1}{\tilde{P}_0(x)}, \qquad P_\ell^{(\mathrm{back})}(x) = c\,\tilde{P}_\ell(x)$$

---

## Why the forward recurrence is stable here

A three-term recurrence has **two linearly independent solutions**. For the
Legendre recurrence these are $P_\ell(x)$ and the Legendre function of the
second kind $Q_\ell(x)$.

For fixed $x \in (-1, 1)$ and large $\ell$, both solutions have similar
asymptotic magnitude — both decay like $O(\ell^{-1/2})$:

$$P_\ell(\cos\theta) \sim \sqrt{\frac{2}{\pi \ell \sin\theta}}
  \cos\!\left((\ell+\tfrac{1}{2})\theta - \tfrac{\pi}{4}\right)$$

$$Q_\ell(\cos\theta) \sim \sqrt{\frac{\pi}{2\ell \sin\theta}}
  \sin\!\left((\ell+\tfrac{1}{2})\theta - \tfrac{\pi}{4}\right)$$

Since neither solution **dominates** the other in the forward direction, any
small contamination of $P_\ell$ by $Q_\ell$ does not grow without bound. The
forward recurrence is therefore **stable** for $|x| < 1$, and errors stay near
machine precision throughout.

This is confirmed by the plots: the forward relative error remains flat around
$10^{-15}$–$10^{-16}$ for all tested $x$ values. The apparent spikes at
certain $\ell$ occur at zeros of $P_\ell(x)$ where the denominator of the
relative error is near zero; the absolute error at those points is still tiny.

---

## Why the backward recurrence fails

Miller's algorithm is powerful **only when the desired solution is minimal in
the direction of backward propagation** — meaning it is exponentially smaller
than the other independent solution going backward. In that case, the other
("dominant") solution gets killed off by the backward iteration and only the
minimal solution survives.

For $P_\ell(x)$ on $|x| < 1$, this condition does not hold: the ratio
$P_\ell / Q_\ell$ stays bounded as $\ell$ increases (both solutions have the
same order of magnitude). The arbitrary starting values $\tilde P_L = 1$,
$\tilde P_{L+1} = 0$ introduce a mixture of $P_\ell$ and $Q_\ell$ that does
not simplify through backward propagation. After rescaling, the result is a
linear combination of the two solutions, not $P_\ell$ alone.

The plots confirm this: the backward relative error is $O(10^{-2})$–$O(10^{-1})$
across all $\ell$, with no improvement at small $\ell$.

Miller's algorithm **would** be appropriate for, e.g., modified Bessel
functions $I_\ell$ / $K_\ell$, where the solutions have genuinely different
growth rates and the desired solution is exponentially minimal.

---

## Connection to spherical harmonics

The zonal spherical harmonics are related to $P_\ell$ by:

$$Y_{\ell 0}(\theta, \phi) = \sqrt{\frac{2\ell+1}{4\pi}}\,P_\ell(\cos\theta)$$

The normalization factor is known exactly. Any numerical error in $P_\ell(\cos\theta)$
propagates directly into $Y_{\ell 0}$, scaled by $\sqrt{(2\ell+1)/4\pi}$.
For $\ell = 50$ this factor is about 2.0, so the absolute error in $Y_{50,0}$
is roughly twice the absolute error in $P_{50}(\cos\theta)$ — still at machine
precision for the forward recurrence.

---

## High-precision reference

`scripts/reference.py` uses `mpmath` with `mp.dps = 50` (50 decimal digits ≈
166 bits), far beyond the ~15 digits of IEEE 754 double precision. The
`mpmath.legendre(l, x)` function evaluates $P_\ell(x)$ at arbitrary precision
using the hypergeometric representation. These values are treated as exact for
the purpose of error analysis.

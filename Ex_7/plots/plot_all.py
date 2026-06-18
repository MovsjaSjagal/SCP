"""
plots/plot_all.py
Generate all Ex_7 plots from the CSV files in output/.
Run via:  make plot-python
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os

plt.style.use("seaborn-v0_8-whitegrid")
plt.rcParams["figure.figsize"] = (8, 4.5)
plt.rcParams["axes.spines.top"] = False
plt.rcParams["axes.spines.right"] = False
os.makedirs("output", exist_ok=True)

# ── Exercise 1: LLN ───────────────────────────────────────────────────────────
data = np.loadtxt("output/coin_toss.csv", delimiter=",", skiprows=1)
n, frac = data[:, 0], data[:, 1]

fig, ax = plt.subplots()
ax.plot(n, frac, color="#4C78A8", linewidth=1.0, label="Running fraction")
ax.axhline(0.5, color="#E45756", linestyle="--", linewidth=1.8, label="Expected 0.5")
ax.set_xscale("log")
ax.set_ylim(0.3, 0.7)
ax.set_xlabel("Number of tosses (n)")
ax.set_ylabel("Fraction of heads")
ax.set_title("Exercise 1: Law of Large Numbers — Coin Toss")
ax.legend()
plt.tight_layout()
plt.savefig("output/ex1_coin_toss.png", dpi=150)
plt.close()
print("Saved output/ex1_coin_toss.png")

# ── Exercise 2: Monte Carlo π ─────────────────────────────────────────────────
data = np.loadtxt("output/monte_carlo_pi.csv", delimiter=",", skiprows=1)
N, pi_est, err = data[:, 0], data[:, 1], data[:, 2]

fig, ax = plt.subplots()
ax.plot(N, err, "o-", color="#4C78A8", linewidth=2, markersize=7, label="MC error")
ref = 1.8 / np.sqrt(N)
ax.plot(N, ref, "--", color="#E45756", linewidth=1.8, label=r"$\sim 1/\sqrt{N}$ reference")
ax.set_xscale("log")
ax.set_yscale("log")
ax.set_xlabel("N (samples)")
ax.set_ylabel(r"$|\hat{\pi} - \pi|$")
ax.set_title(r"Exercise 2: Monte Carlo Estimate of $\pi$ — Error vs N")
ax.legend()
plt.tight_layout()
plt.savefig("output/ex2_monte_carlo_pi.png", dpi=150)
plt.close()
print("Saved output/ex2_monte_carlo_pi.png")

# ── Exercise 3: Change of variables ──────────────────────────────────────────
data = np.loadtxt("output/change_of_variables.csv", delimiter=",", skiprows=1)
y_mid, emp, ana = data[:, 0], data[:, 1], data[:, 2]

fig, ax = plt.subplots()
ax.bar(y_mid, emp, width=(y_mid[1] - y_mid[0]) * 0.9,
       color="#4C78A8", alpha=0.65, label="Empirical histogram")
ax.plot(y_mid, ana, color="#E45756", linewidth=2.5,
        label=r"Analytic $f_Y(y) = 1/(2\sqrt{y})$")
ax.set_xlim(0, 1)
ax.set_ylim(0, min(ana.max() * 1.1, 15))
ax.set_xlabel("y")
ax.set_ylabel("Density")
ax.set_title(r"Exercise 3: Transformation $Y = U^2$")
ax.legend()
plt.tight_layout()
plt.savefig("output/ex3_change_of_variables.png", dpi=150)
plt.close()
print("Saved output/ex3_change_of_variables.png")

# ── Exercise 4: Inverse transform ────────────────────────────────────────────
data = np.loadtxt("output/exponential_pdf.csv", delimiter=",", skiprows=1)
y_mid, emp, ana = data[:, 0], data[:, 1], data[:, 2]

fig, ax = plt.subplots()
ax.bar(y_mid, emp, width=(y_mid[1] - y_mid[0]) * 0.9,
       color="#72B7B2", alpha=0.65, label="Empirical histogram")
ax.plot(y_mid, ana, color="#E45756", linewidth=2.5,
        label=r"PDF: $\lambda e^{-\lambda y}$, $\lambda=1.5$")
ax.set_xlabel("y")
ax.set_ylabel("Density")
ax.set_title(r"Exercise 4: Inverse Transform — Exponential($\lambda=1.5$)")
ax.legend()
plt.tight_layout()
plt.savefig("output/ex4_exponential_pdf.png", dpi=150)
plt.close()
print("Saved output/ex4_exponential_pdf.png")

# ── Exercise 5: Empirical CDF ─────────────────────────────────────────────────
data = np.loadtxt("output/empirical_cdf.csv", delimiter=",", skiprows=1)
y, ecdf, exact = data[:, 0], data[:, 1], data[:, 2]

fig, ax = plt.subplots()
ax.step(y, ecdf, color="#4C78A8", linewidth=1.5, where="post", label="Empirical CDF")
ax.plot(y, exact, "--", color="#E45756", linewidth=2.5,
        label=r"Exact: $1 - e^{-\lambda y}$")
ax.set_ylim(0, 1)
ax.set_xlabel("y")
ax.set_ylabel("F(y)")
ax.set_title(r"Exercise 5: Empirical vs Exact CDF — Exponential($\lambda=1.5$)")
ax.legend()
plt.tight_layout()
plt.savefig("output/ex5_empirical_cdf.png", dpi=150)
plt.close()
print("Saved output/ex5_empirical_cdf.png")

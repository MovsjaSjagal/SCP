"""
scripts/plot_errors.py
Load C recurrence output and mpmath reference, compute errors, plot.
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os

plt.style.use("seaborn-v0_8-whitegrid")
plt.rcParams["axes.spines.top"]   = False
plt.rcParams["axes.spines.right"] = False
os.makedirs("output", exist_ok=True)

# ── Load data ─────────────────────────────────────────────────────────────────
rec = np.loadtxt("output/legendre_recurrence.csv", delimiter=",", skiprows=1)
ref = np.loadtxt("output/legendre_reference.csv",  delimiter=",", skiprows=1,
                 dtype=str)

# Convert reference to float (high-precision strings → float64)
x_ref = ref[:, 0].astype(float)
l_ref = ref[:, 1].astype(int)
P_ref = ref[:, 2].astype(float)

x_rec   = rec[:, 0]
l_rec   = rec[:, 1].astype(int)
P_fwd   = rec[:, 2]
P_bwd   = rec[:, 3]

X_VALS = [0.1, 0.5, 0.9, 0.99]
EPS    = 1e-300    # guard against division by zero near polynomial zeros

# ── Plot 1: relative errors (2×2 grid) ───────────────────────────────────────
fig, axes = plt.subplots(2, 2, figsize=(12, 9))
axes = axes.flatten()

for idx, x in enumerate(X_VALS):
    mask_rec = x_rec == x
    mask_ref = x_ref == x
    l    = l_rec[mask_rec]
    Pf   = P_fwd[mask_rec]
    Pb   = P_bwd[mask_rec]
    Pr   = P_ref[mask_ref]

    denom = np.maximum(np.abs(Pr), EPS)
    err_fwd = np.abs(Pf - Pr) / denom
    err_bwd = np.abs(Pb - Pr) / denom

    ax = axes[idx]
    ax.semilogy(l, err_fwd, "o-", color="#4C78A8", linewidth=1.5,
                markersize=4, label="Forward recurrence")
    ax.semilogy(l, err_bwd, "s--", color="#E45756", linewidth=1.5,
                markersize=4, label="Backward (Miller)")
    ax.axhline(1e-15, color="gray", linestyle=":", linewidth=1,
               label="Machine $\\varepsilon$")
    ax.set_title(f"$x = {x}$")
    ax.set_xlabel("$\\ell$")
    ax.set_ylabel("Relative error")
    ax.legend(fontsize=8)

fig.suptitle("Relative error vs $\\ell$: forward vs backward recurrence", fontsize=13)
plt.tight_layout()
plt.savefig("output/relative_errors.png", dpi=150)
plt.close()
print("Saved output/relative_errors.png")

# ── Plot 2: absolute errors (2×2 grid) ───────────────────────────────────────
fig, axes = plt.subplots(2, 2, figsize=(12, 9))
axes = axes.flatten()

for idx, x in enumerate(X_VALS):
    mask_rec = x_rec == x
    mask_ref = x_ref == x
    l    = l_rec[mask_rec]
    Pf   = P_fwd[mask_rec]
    Pb   = P_bwd[mask_rec]
    Pr   = P_ref[mask_ref]

    err_fwd = np.abs(Pf - Pr)
    err_bwd = np.abs(Pb - Pr)

    ax = axes[idx]
    ax.semilogy(l, np.maximum(err_fwd, 1e-20), "o-", color="#4C78A8",
                linewidth=1.5, markersize=4, label="Forward recurrence")
    ax.semilogy(l, np.maximum(err_bwd, 1e-20), "s--", color="#E45756",
                linewidth=1.5, markersize=4, label="Backward (Miller)")
    ax.set_title(f"$x = {x}$")
    ax.set_xlabel("$\\ell$")
    ax.set_ylabel("Absolute error")
    ax.legend(fontsize=8)

fig.suptitle("Absolute error vs $\\ell$: forward vs backward recurrence", fontsize=13)
plt.tight_layout()
plt.savefig("output/absolute_errors.png", dpi=150)
plt.close()
print("Saved output/absolute_errors.png")

# ── Plot 3: P_l values themselves for reference ───────────────────────────────
fig, axes = plt.subplots(2, 2, figsize=(12, 9))
axes = axes.flatten()

for idx, x in enumerate(X_VALS):
    mask_ref = x_ref == x
    l  = l_ref[mask_ref]
    Pr = P_ref[mask_ref]

    ax = axes[idx]
    ax.plot(l, Pr, "o-", color="#4C78A8", linewidth=1.5, markersize=4)
    ax.axhline(0, color="gray", linestyle=":", linewidth=1)
    ax.set_title(f"$P_\\ell({x})$")
    ax.set_xlabel("$\\ell$")
    ax.set_ylabel("$P_\\ell(x)$")

fig.suptitle("Legendre polynomial values $P_\\ell(x)$", fontsize=13)
plt.tight_layout()
plt.savefig("output/legendre_values.png", dpi=150)
plt.close()
print("Saved output/legendre_values.png")

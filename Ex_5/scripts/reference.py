"""
scripts/reference.py
Compute high-precision Legendre polynomial values using mpmath.
mp.dps = 50 gives ~50 decimal digits (~166 bits), far beyond double precision.
Output: output/legendre_reference.csv
"""

import mpmath
import csv
import os

mpmath.mp.dps = 50      # 50 decimal digits of working precision

X_VALS = [0.1, 0.5, 0.9, 0.99]
L_MAX  = 50

os.makedirs("output", exist_ok=True)

with open("output/legendre_reference.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["x", "l", "P_ref"])
    for x in X_VALS:
        xmp = mpmath.mpf(x)
        for l in range(L_MAX + 1):
            P = mpmath.legendre(l, xmp)
            writer.writerow([f"{x:.2f}", l, mpmath.nstr(P, 35)])

print("High-precision reference written to output/legendre_reference.csv")

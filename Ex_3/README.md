# Matrix Multiplication Performance (C)

## Description
This program benchmarks different matrix multiplication strategies:
- ijk loop ordering
- ikj loop ordering (cache optimized)
- flat matrix (1D array)

## Usage
Compile:
    make

Run:
    ./program 2.0 3.0 1000 result

## Output
Generates:
- result.txt
- result.csv
- result.bin

## Notes
- Matrices are diagonal for fast validation
- Performance differences highlight cache effects

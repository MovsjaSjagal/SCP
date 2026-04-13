// =============================================================================
// matrix_io.c
// Homework 03 — Matrix Multiplication Performance
//
// Implementation of all matrix creation, multiplication, validation,
// and export functions declared in matrix_io.h.
// =============================================================================

#include "matrix_io.h"

#include <stdio.h>   // printf, fprintf, FILE, fopen, fclose, fwrite
#include <stdlib.h>  // malloc, free, NULL
#include <math.h>    // sqrt, fabs
#include <time.h>    // clock(), CLOCKS_PER_SEC


// -----------------------------------------------------------------------------
// Matrix/vector creation
// -----------------------------------------------------------------------------

double** createDiagonalMatrix(int N, double a) {
    // Allocate array of row pointers
    double** matrix = (double**)malloc(N * sizeof(double*));
    if (matrix == NULL) {
        fprintf(stderr, "Error: failed to allocate row pointers.\n");
        return NULL;
    }

    // Allocate each row and zero-initialize
    for (int i = 0; i < N; i++) {
        matrix[i] = (double*)calloc(N, sizeof(double));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Error: failed to allocate row %d.\n", i);
            // Free already-allocated rows before returning
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }

    // Set diagonal elements
    for (int i = 0; i < N; i++)
        matrix[i][i] = a;

    return matrix;
}

double* createDiagonalVector(int N, double a) {
    // Allocate and zero-initialize a flat N*N array
    double* vec = (double*)calloc(N * N, sizeof(double));
    if (vec == NULL) {
        fprintf(stderr, "Error: failed to allocate diagonal vector.\n");
        return NULL;
    }

    // Set logical diagonal elements: element (i,i) is at index i*N + i
    for (int i = 0; i < N; i++)
        vec[i + i * N] = a;

    return vec;
}

void freeMatrix(double** matrix, int N) {
    for (int i = 0; i < N; i++)
        free(matrix[i]);
    free(matrix);
}


// -----------------------------------------------------------------------------
// Matrix multiplication
// -----------------------------------------------------------------------------

double** multiplyMatrix_ijk(double** matrix1, double** matrix2, int N) {
    // Allocate result matrix
    double** result = createDiagonalMatrix(N, 0.0);
    if (result == NULL) return NULL;

    // Zero out the diagonal (createDiagonalMatrix sets diagonal to 0.0 here,
    // but calloc already handles that — result is a clean zero matrix)

    clock_t start = clock();

    // Standard i-j-k ordering: poor cache locality on matrix2 (column-wise access)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                result[i][j] += matrix1[i][k] * matrix2[k][j];

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total computation time of matrix ijk-multiplication was: %.6f seconds.\n", elapsed);

    return result;
}

double** multiplyMatrix_ikj(double** matrix1, double** matrix2, int N) {
    double** result = createDiagonalMatrix(N, 0.0);
    if (result == NULL) return NULL;

    clock_t start = clock();

    // i-k-j ordering: inner loop accesses matrix2 and result row-wise (cache-friendly)
    for (int i = 0; i < N; i++)
        for (int k = 0; k < N; k++) {
            double r = matrix1[i][k];
            for (int j = 0; j < N; j++)
                result[i][j] += r * matrix2[k][j];
        }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total computation time of matrix ikj-multiplication was: %.6f seconds.\n", elapsed);

    return result;
}

double* multiplyVector(double* vector1, double* vector2, int N) {
    // Allocate and zero-initialize result vector
    double* result = (double*)calloc(N * N, sizeof(double));
    if (result == NULL) {
        fprintf(stderr, "Error: failed to allocate result vector.\n");
        return NULL;
    }

    clock_t start = clock();

    // i-k-j ordering on flat row-major arrays: optimal cache locality
    for (int i = 0; i < N; i++)
        for (int k = 0; k < N; k++) {
            double r = vector1[i * N + k];
            for (int j = 0; j < N; j++)
                result[i * N + j] += r * vector2[k * N + j];
        }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total computation time of vector multiplication was: %.6f seconds.\n", elapsed);

    return result;
}


// -----------------------------------------------------------------------------
// Matrix validation
// -----------------------------------------------------------------------------

int checkMultiplication(double** matrix, int N, double a, double b) {
    double expected = a * b;

    clock_t start = clock();

    for (int i = 0; i < N; i++) {
        if (fabs(matrix[i][i] - expected) > 1e-12) {
            return 0;
        }
    }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total computation time of matrix diagonal check: %.6f seconds.\n", elapsed);

    return 1;
}

int checkMultiplicationVector(double* vec, int N, double a, double b) {
    double expected = a * b;

    clock_t start = clock();

    for (int i = 0; i < N; i++) {
        if (fabs(vec[i * N + i] - expected) > 1e-12) {
            return 0;
        }
    }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total computation time of vector diagonal check: %.6f seconds.\n", elapsed);

    return 1;
}

int crossCheckVectorMatrix(double* vec, double** matrix, int N) {
    clock_t start = clock();

    for (int i = 0; i < N; i++) {
        if (fabs(vec[i * N + i] - matrix[i][i]) > 1e-12) {
            return 0;
        }
    }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total computation time of cross-check: %.6f seconds.\n", elapsed);

    return 1;
}


// -----------------------------------------------------------------------------
// Matrix export
// -----------------------------------------------------------------------------

void write_matrix_txt(double** M, int N, const char* filename) {
    FILE* out = fopen(filename, "w");
    if (out == NULL) {
        fprintf(stderr, "Error: could not open file '%s' for writing.\n", filename);
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            fprintf(out, "%.6f ", M[i][j]);
        fprintf(out, "\n");
    }

    fclose(out);
}

void write_matrix_csv(double** M, int N, const char* filename) {
    FILE* out = fopen(filename, "w");
    if (out == NULL) {
        fprintf(stderr, "Error: could not open file '%s' for writing.\n", filename);
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(out, "%.6f", M[i][j]);
            if (j < N - 1) fprintf(out, ",");
        }
        fprintf(out, "\n");
    }

    fclose(out);
}

void write_matrix_bin(double** M, int N, const char* filename) {
    FILE* out = fopen(filename, "wb");
    if (out == NULL) {
        fprintf(stderr, "Error: could not open file '%s' for writing.\n", filename);
        return;
    }

    // Write dimensions first so the file can be reconstructed
    size_t rows = (size_t)N;
    size_t cols = (size_t)N;
    fwrite(&rows, sizeof(size_t), 1, out);
    fwrite(&cols, sizeof(size_t), 1, out);

    // Write each row as a contiguous block of doubles
    for (int i = 0; i < N; i++)
        fwrite(M[i], sizeof(double), (size_t)N, out);

    fclose(out);
}

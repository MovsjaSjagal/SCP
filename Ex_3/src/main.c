// =============================================================================
// main.c
// Homework 03 — Matrix Multiplication Performance
//
// Investigates the compute time of matrix multiplication using different
// loop orderings (i-j-k vs i-k-j) and memory layouts (2D pointer array
// vs 1D flat contiguous array).
//
// Structure:
//   1) Create two N x N diagonal matrices and their flat 1D equivalents.
//   2) Multiply using three strategies: ijk, ikj, and flat vector ikj.
//   3) Validate all results against the expected diagonal value a * b.
//   4) Cross-validate the 1D and 2D results against each other.
//   5) Export the ijk result to .txt, .csv, and .bin formats.
//
// =============================================================================

#include "matrix_io.h"
#include <stdio.h>   // printf
#include <stdlib.h>  // free


int main(int argc, char* argv[]) {

    // -------------------------------------------------------------------------
    // 1) Obtain parameters
    // -------------------------------------------------------------------------

    double a = 2.0;
    double b = 3.0;
    int N = 1000;
    const char* fileout = "result";

    if (argc >= 5) {
        a = atof(argv[1]);
        b = atof(argv[2]);
        N = atoi(argv[3]);
        fileout = argv[4];
        printf("Using command line input to start program.\n");
    }
    else {
        printf("Using default values (no command-line input provided)\n");
    }

    // -------------------------------------------------------------------------
    // 2) Create diagonal matrices and flat vectors
    // -------------------------------------------------------------------------
    double** matrix1 = createDiagonalMatrix(N, a);
    double** matrix2 = createDiagonalMatrix(N, b);
    double* vector1 = createDiagonalVector(N, a);
    double* vector2 = createDiagonalVector(N, b);

    if (matrix1 == NULL || matrix2 == NULL || vector1 == NULL || vector2 == NULL) {
        fprintf(stderr, "Error: allocation failed during matrix/vector creation.\n");
        return 1;
    }

    // -------------------------------------------------------------------------
    // 3) Multiply using three strategies
    // -------------------------------------------------------------------------
    double** matrixProduct_ijk = multiplyMatrix_ijk(matrix1, matrix2, N);
    double** matrixProduct_ikj = multiplyMatrix_ikj(matrix1, matrix2, N);
    double* vectorProduct = multiplyVector(vector1, vector2, N);

    if (matrixProduct_ijk == NULL || matrixProduct_ikj == NULL || vectorProduct == NULL) {
        fprintf(stderr, "Error: allocation failed during multiplication.\n");
        return 1;
    }

    // -------------------------------------------------------------------------
    // 4) Validate results and export if correct
    // -------------------------------------------------------------------------
    if (checkMultiplication(matrixProduct_ijk, N, a, b)
        && checkMultiplication(matrixProduct_ikj, N, a, b)
        && checkMultiplicationVector(vectorProduct, N, a, b)
        && crossCheckVectorMatrix(vectorProduct, matrixProduct_ijk, N)) {

        printf("Multiplication done correctly. Exporting files.\n");

        char filename_txt[256];
        char filename_csv[256];
        char filename_bin[256];

        snprintf(filename_txt, sizeof(filename_txt), "%s.txt", fileout);
        snprintf(filename_csv, sizeof(filename_csv), "%s.csv", fileout);
        snprintf(filename_bin, sizeof(filename_bin), "%s.bin", fileout);

        write_matrix_txt(matrixProduct_ijk, N, filename_txt);
        write_matrix_csv(matrixProduct_ijk, N, filename_csv);
        write_matrix_bin(matrixProduct_ijk, N, filename_bin);
    }
    else {
        printf("Multiplication erroneous. No files produced.\n");
    }

    // -------------------------------------------------------------------------
    // 5) Free all dynamically allocated memory
    // -------------------------------------------------------------------------
    freeMatrix(matrix1, N);
    freeMatrix(matrix2, N);
    freeMatrix(matrixProduct_ijk, N);
    freeMatrix(matrixProduct_ikj, N);
    free(vector1);
    free(vector2);
    free(vectorProduct);

    return 0;
}

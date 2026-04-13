// =============================================================================
// matrix_io.h
// Homework 03 — Matrix Multiplication Performance
//
// Header file declaring all matrix creation, multiplication, validation,
// and export functions used in this program.
//
// Matrices are represented in two ways:
//   (1) 2D: a double** array of N pointers, each pointing to a row of N doubles.
//   (2) 1D: a flat double* array of N*N doubles in row-major order,
//           where element (i,j) is accessed as vec[i*N + j].
//
// All functions that return dynamically allocated memory are documented
// accordingly. The caller is responsible for freeing that memory.
// =============================================================================

#ifndef MATRIX_IO_H
#define MATRIX_IO_H

#include <stddef.h>  // size_t


// -----------------------------------------------------------------------------
// Matrix/vector creation functions
// -----------------------------------------------------------------------------

/*
 * Function: createDiagonalMatrix
 * ------------------------------
 * Allocates and initializes an N x N diagonal matrix where all main diagonal
 * elements are set to scalar a, and all other elements are zero.
 *
 * Parameters:
 *   N (int)    : Number of rows and columns.
 *   a (double) : Scalar value placed on the main diagonal.
 *
 * Returns:
 *   double** : Heap-allocated 2D array. The caller must free each row and
 *              then the pointer array itself (see freeMatrix()).
 *
 * Complexity:
 *   Time:  O(N^2) — full zero-initialization plus O(N) diagonal assignment.
 *   Space: O(N^2) — non-contiguous heap blocks (array of row pointers).
 */
double** createDiagonalMatrix(int N, double a);

/*
 * Function: createDiagonalVector
 * ------------------------------
 * Allocates and initializes an N x N diagonal matrix flattened into a
 * 1D row-major array of size N*N. Element (i,j) is at index i*N + j.
 *
 * Parameters:
 *   N (int)    : Logical dimension of the matrix (output size is N*N).
 *   a (double) : Scalar value placed on the logical main diagonal.
 *
 * Returns:
 *   double* : Heap-allocated 1D array of size N*N. Caller must free().
 *
 * Complexity:
 *   Time:  O(N^2) — full zero-initialization plus O(N) diagonal assignment.
 *   Space: O(N^2) — single contiguous block of memory.
 */
double* createDiagonalVector(int N, double a);

/*
 * Function: freeMatrix
 * --------------------
 * Frees a heap-allocated 2D matrix created by createDiagonalMatrix().
 * Must be called for every double** matrix when it is no longer needed.
 *
 * Parameters:
 *   matrix (double**) : The matrix to free.
 *   N (int)           : The number of rows (used to free each row).
 */
void freeMatrix(double** matrix, int N);


// -----------------------------------------------------------------------------
// Matrix multiplication functions
// -----------------------------------------------------------------------------

/*
 * Function: multiplyMatrix_ijk
 * ----------------------------
 * Performs naive matrix multiplication using standard i-j-k loop ordering.
 *
 * Parameters:
 *   matrix1 (double**) : N x N left operand.
 *   matrix2 (double**) : N x N right operand.
 *   N (int)            : Matrix dimension.
 *
 * Returns:
 *   double** : Heap-allocated N x N result matrix. Caller must freeMatrix().
 *              Returns NULL on dimension mismatch or allocation failure.
 *
 * Performance:
 *   Time Complexity: O(N^3)
 *   Cache Locality:  Poor. The innermost loop (k) traverses matrix2 column-wise,
 *                    causing frequent cache misses in row-major memory layout.
 */
double** multiplyMatrix_ijk(double** matrix1, double** matrix2, int N);

/*
 * Function: multiplyMatrix_ikj
 * ----------------------------
 * Performs cache-optimized matrix multiplication using i-k-j loop ordering.
 *
 * Parameters:
 *   matrix1 (double**) : N x N left operand.
 *   matrix2 (double**) : N x N right operand.
 *   N (int)            : Matrix dimension.
 *
 * Returns:
 *   double** : Heap-allocated N x N result matrix. Caller must freeMatrix().
 *              Returns NULL on allocation failure.
 *
 * Performance:
 *   Time Complexity: O(N^3)
 *   Cache Locality:  Excellent. The innermost loop (j) accesses matrix2 and
 *                    the result matrix strictly row-wise, maximizing spatial
 *                    locality and CPU prefetch effectiveness.
 */
double** multiplyMatrix_ikj(double** matrix1, double** matrix2, int N);

/*
 * Function: multiplyVector
 * ------------------------
 * Computes the matrix product of two N x N matrices stored as flat 1D
 * row-major arrays. Uses i-k-j loop ordering for cache efficiency.
 *
 * Parameters:
 *   vector1 (double*) : Left operand, flattened array of size N*N.
 *   vector2 (double*) : Right operand, flattened array of size N*N.
 *   N (int)           : Logical matrix dimension.
 *
 * Returns:
 *   double* : Heap-allocated result array of size N*N. Caller must free().
 *             Returns NULL on allocation failure.
 *
 * Performance:
 *   Time Complexity: O(N^3)
 *   Cache Locality:  Optimal. Combines i-k-j row-wise traversal with a
 *                    strictly contiguous memory layout, eliminating the
 *                    pointer-chasing overhead of double**.
 */
double* multiplyVector(double* vector1, double* vector2, int N);


// -----------------------------------------------------------------------------
// Matrix validation functions
// -----------------------------------------------------------------------------

/*
 * Function: checkMultiplication
 * -----------------------------
 * Validates a 2D matrix multiplication result by verifying that all main
 * diagonal elements equal the expected scalar product a * b, within a
 * floating-point tolerance of 1e-12.
 *
 * Parameters:
 *   matrix (double**) : The N x N result matrix to verify.
 *   N (int)           : Matrix dimension.
 *   a (double)        : Diagonal scalar of the first input matrix.
 *   b (double)        : Diagonal scalar of the second input matrix.
 *
 * Returns:
 *   int : 1 if all diagonal elements match a*b within tolerance, 0 otherwise.
 */
int checkMultiplication(double** matrix, int N, double a, double b);

/*
 * Function: checkMultiplicationVector
 * ------------------------------------
 * Validates a 1D flattened matrix multiplication result by verifying that
 * all logical main diagonal elements equal a * b within tolerance.
 *
 * Parameters:
 *   vec (double*) : Flattened N*N result array to verify.
 *   N (int)       : Logical matrix dimension.
 *   a (double)    : Diagonal scalar of the first input matrix.
 *   b (double)    : Diagonal scalar of the second input matrix.
 *
 * Returns:
 *   int : 1 if all diagonal elements match a*b within tolerance, 0 otherwise.
 */
int checkMultiplicationVector(double* vec, int N, double a, double b);

/*
 * Function: crossCheckVectorMatrix
 * ---------------------------------
 * Cross-validates the 1D contiguous result against the 2D result by
 * comparing their logical main diagonals within a tolerance of 1e-12.
 *
 * Parameters:
 *   vec    (double**)  : Flattened N*N result array.
 *   matrix (double**)  : 2D N x N result matrix.
 *   N      (int)       : Matrix dimension.
 *
 * Returns:
 *   int : 1 if diagonals match within tolerance, 0 otherwise.
 */
int crossCheckVectorMatrix(double* vec, double** matrix, int N);


// -----------------------------------------------------------------------------
// Matrix export functions
// -----------------------------------------------------------------------------

/*
 * Function: write_matrix_txt
 * --------------------------
 * Writes a 2D matrix to a plain text file.
 *
 * Format:
 *   Each row on a new line, elements separated by spaces.
 *   Example:
 *     1.000000 2.000000 3.000000
 *     4.000000 5.000000 6.000000
 *
 * Parameters:
 *   M        (double**) : Matrix to write.
 *   N        (int)      : Matrix dimension.
 *   filename (char*)    : Output filename.
 *
 * Notes:
 *   Human-readable and easy to debug; inefficient for large matrices.
 */
void write_matrix_txt(double** M, int N, const char* filename);

/*
 * Function: write_matrix_csv
 * --------------------------
 * Writes a 2D matrix to a CSV (Comma-Separated Values) file.
 *
 * Format:
 *   Each row on a new line, elements separated by commas.
 *   Example:
 *     1.000000,2.000000,3.000000
 *     4.000000,5.000000,6.000000
 *
 * Parameters:
 *   M        (double**) : Matrix to write.
 *   N        (int)      : Matrix dimension.
 *   filename (char*)    : Output filename.
 *
 * Notes:
 *   Compatible with Excel, Python, MATLAB. Still text-based — large files
 *   and slower I/O compared to binary format.
 */
void write_matrix_csv(double** M, int N, const char* filename);

/*
 * Function: write_matrix_bin
 * --------------------------
 * Writes a 2D matrix to a binary file.
 *
 * Format (must be known to read back correctly):
 *   [N as size_t][N as size_t][row0 doubles][row1 doubles]...[rowN-1 doubles]
 *
 * Parameters:
 *   M        (double**) : Matrix to write (must be non-empty and rectangular).
 *   N        (int)      : Matrix dimension.
 *   filename (char*)    : Output filename.
 *
 * Advantages:
 *   Fast I/O, compact storage, no floating-point precision loss.
 *
 * Drawbacks:
 *   Not human-readable. Format depends on system architecture
 *   (size_t width, endianness).
 */
void write_matrix_bin(double** M, int N, const char* filename);

#endif

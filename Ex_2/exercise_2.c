// =============================================================================
// exercise_2.c
// Homework 02 — Linear Vector Operation
//
// Computes the linear combination:
//
//     d = a * v1 + v2
//
// where a is a user-supplied scalar and v1, v2 are uniform vectors of
// user-supplied dimension N (all elements of each vector are equal).
// The result vector d is also uniform; only the first element is printed,
// since all elements are identical by construction.
//
// Usage:
//   Compile:  gcc exercise_2.c -o exercise_2
//   Run:      ./exercise_2
//
// Input (interactive, via stdin):
//   - a         : scalar (double)
//   - v1 value  : common element value for vector v1 (double)
//   - v2 value  : common element value for vector v2 (double)
//   - N         : dimension of the vectors (int)
//
// Output:
//   - The common element value of the result vector d = a*v1 + v2 (double)
//
// Notes:
//   - Memory for v1, v2, and result is allocated dynamically with malloc().
//     For very large N, this may exceed available RAM (see README.md).
//   - Tested with N = 10, 10^6, 10^8. N >= 10^9 causes memory exhaustion.
// =============================================================================

#include <stdio.h>   // printf, scanf
#include <stdlib.h>  // malloc, free

// -----------------------------------------------------------------------------
// OS-specific sleep support
// -----------------------------------------------------------------------------
#ifdef _WIN32
#include <windows.h> // Sleep() on Windows
#else
#include <unistd.h>  // sleep() on POSIX (macOS/Linux)
#endif


// -----------------------------------------------------------------------------
// delay_seconds()
//
// Portable wrapper around OS-specific sleep functions.
//
// Parameters:
//   seconds (int) : number of seconds to pause execution
// -----------------------------------------------------------------------------
void delay_seconds(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000); // Windows Sleep() takes milliseconds
#else
    sleep(seconds);        // POSIX sleep() takes seconds
#endif
}


// -----------------------------------------------------------------------------
// printExplanation()
//
// Prints a brief description of the program and the operation it performs
// to stdout. Called once at program start.
// -----------------------------------------------------------------------------
void printExplanation() {
    printf("-------------------------------------------------------------\n");
    printf("  Linear Vector Operation: d = a * v1 + v2\n");
    printf("-------------------------------------------------------------\n");
    printf("This program computes the linear combination d = a*v1 + v2.\n");
    printf("All elements of v1 and v2 are equal (uniform vectors).\n");
    printf("You will be asked to provide: a, the element value of v1,\n");
    printf("the element value of v2, and the vector dimension N.\n\n");
}


// -----------------------------------------------------------------------------
// getNumbers()
//
// Prompts the user for the scalar a and the uniform element values of
// vectors v1 and v2. Stores the three values in the provided array.
//
// Parameters:
//   numbers (double*) : pre-allocated array of length 3.
//                       On return: numbers[0] = a,
//                                  numbers[1] = v1 element,
//                                  numbers[2] = v2 element.
// -----------------------------------------------------------------------------
void getNumbers(double* numbers) {
    double a = 0;
    printf("Enter the value of the scalar a: ");
    scanf("%lf", &a);

    double v1 = 0;
    printf("Enter the element value for vector v1: ");
    scanf("%lf", &v1);

    double v2 = 0;
    printf("Enter the element value for vector v2: ");
    scanf("%lf", &v2);

    numbers[0] = a;
    numbers[1] = v1;
    numbers[2] = v2;
}


// -----------------------------------------------------------------------------
// getDimension()
//
// Prompts the user for the vector dimension N.
//
// Returns:
//   (int) : the requested vector dimension N
// -----------------------------------------------------------------------------
int getDimension() {
    int dimension = 0;
    printf("Enter the vector dimension N: ");
    scanf("%d", &dimension);
    return dimension;
}


// -----------------------------------------------------------------------------
// printResult()
//
// Prints the result of the linear combination to stdout.
// Since all elements of d are identical by construction, only the common
// element value is printed.
//
// Parameters:
//   x (double) : the common element value of the result vector d
// -----------------------------------------------------------------------------
void printResult(double x) {
    printf("\nResult: d[i] = %f  (all elements are equal)\n", x);
}


// -----------------------------------------------------------------------------
// main()
//
// Entry point. Orchestrates user input, memory allocation, computation,
// and output. Frees all dynamically allocated memory before exit.
// -----------------------------------------------------------------------------
int main() {
    // 1) Print program description
    printExplanation();

    // 2) Collect scalar and vector element values from the user
    double user_input[3];  // [0] = a, [1] = v1 element, [2] = v2 element
    getNumbers(user_input);

    // 3) Collect vector dimension from the user
    int dim = getDimension();

    // 4) Allocate memory for v1, v2, and result vectors
    double* v1     = (double*)malloc(dim * sizeof(double));
    double* v2     = (double*)malloc(dim * sizeof(double));
    double* result = (double*)malloc(dim * sizeof(double));

    if (v1 == NULL || v2 == NULL || result == NULL) {
        fprintf(stderr, "Error: memory allocation failed for dimension N = %d.\n", dim);
        free(v1);
        free(v2);
        free(result);
        return 1;
    }

    // 5) Populate v1 and v2 with their uniform element values
    double a = user_input[0];
    for (int i = 0; i < dim; ++i) {
        v1[i] = user_input[1];
        v2[i] = user_input[2];
    }

    // 6) Compute d = a * v1 + v2 element-wise
    for (int i = 0; i < dim; ++i) {
        result[i] = a * v1[i] + v2[i];
    }

    // 7) Print the result
    printResult(result[0]);

    // 8) Free dynamically allocated memory
    free(v1);
    free(v2);
    free(result);

    return 0;
}

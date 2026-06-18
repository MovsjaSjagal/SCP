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
//
// Usage:
//   Compile:  gcc exercise_2.c -o exercise_2
//   Run:      ./exercise_2
//
// Input (interactive, via stdin):
//   - a         : scalar (double)
//   - v1 value  : common element value for vector v1 (double)
//   - v2 value  : common element value for vector v2 (double)
//   - N         : dimension of the vectors (long)
//
// Notes:
//   - Memory for v1, v2, and result is allocated dynamically with malloc().
//     For very large N, this may exceed available RAM (see README.md).
//   - Tested with N = 10, 10^6, 10^8. N >= 10^9 causes memory exhaustion.
// =============================================================================

#include <stdio.h>   // printf, scanf
#include <stdlib.h>  // malloc, free


void printExplanation() {
    printf("-------------------------------------------------------------\n");
    printf("  Linear Vector Operation: d = a * v1 + v2\n");
    printf("-------------------------------------------------------------\n");
    printf("This program computes the linear combination d = a*v1 + v2.\n");
    printf("All elements of v1 and v2 are equal (uniform vectors).\n");
    printf("You will be asked to provide: a, the element value of v1,\n");
    printf("the element value of v2, and the vector dimension N.\n\n");
}


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


long getDimension() {
    long dimension = 0;
    printf("Enter the vector dimension N: ");
    scanf("%ld", &dimension);
    return dimension;
}


int main() {
    printExplanation();

    double user_input[3];  // [0] = a, [1] = v1 element, [2] = v2 element
    getNumbers(user_input);

    long dim = getDimension();

    double* v1     = (double*)malloc(dim * sizeof(double));
    double* v2     = (double*)malloc(dim * sizeof(double));
    double* result = (double*)malloc(dim * sizeof(double));

    if (v1 == NULL || v2 == NULL || result == NULL) {
        fprintf(stderr, "Error: memory allocation failed for dimension N = %ld.\n", dim);
        free(v1);
        free(v2);
        free(result);
        return 1;
    }

    double a = user_input[0];
    for (long i = 0; i < dim; ++i) {
        v1[i] = user_input[1];
        v2[i] = user_input[2];
    }

    for (long i = 0; i < dim; ++i) {
        result[i] = a * v1[i] + v2[i];
    }

    // Verify all elements equal the expected value a*x + y
    double expected = a * user_input[1] + user_input[2];
    int pass = 1;
    for (long i = 0; i < dim; ++i) {
        if (result[i] != expected) { pass = 0; break; }
    }

    printf("\nResult (6 decimal places):  %f\n", result[0]);
    printf("Result (full precision):    %.17f\n", result[0]);
    printf("Verification (all elements == a*x+y): %s\n", pass ? "PASS" : "FAIL");

    // Check exact equality against the mathematical value 7.4 to expose
    // floating-point representation limits (0.1 and 7.1 are not exactly
    // representable in IEEE 754 binary64).
    printf("result[0] == 7.4 exactly?   %s\n",
           (result[0] == 7.4) ? "YES" : "NO  <- floating-point representation error");

    free(v1);
    free(v2);
    free(result);

    return 0;
}

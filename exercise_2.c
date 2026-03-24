// README
// exercise_2.c : This file contains the 'main' function and helper functions of homework two.
//
// The goal of the script is to perform a linear vector operation for given user input values.

// The structure of the program is as follows:

// 1) Explain the user what to do.

// 2) Obtain user input values for scalar and vectors

// 3) Obtain user input values for dimensionality of vectors.

// 4) Perform internal calculation.

// 5) Print result




#include <stdio.h>  // C++ <iostream> replacement for printf and scanf
#include <stdlib.h> // C++ <vector> replacement. Provides malloc() and free() for dynamic memory

// C++ <chrono> and <thread> replacements.
// C handles sleep functions differently depending on the Operating System.
#ifdef _WIN32
#include <windows.h> // For Sleep() on Windows
#else
#include <unistd.h>  // For sleep() on macOS/Linux
#endif

void printExplanation() {
    // std::cout is replaced by printf.
    printf("We will perform a linear vector operation of the type a * v1 + v2, given YOUR input! Each element of each vector will be equal, and the dimension of the vector will need to be specified by you. \n \n");
}

// In C, a function cannot return a full array directly like std::vector.
// Instead, we pass an array (as a pointer) into the function to modify it.
void getNumbers(double* numbers) {
    printf("Please specify the value of the scalar a: ");
    double a = 0; // C doesn't support {} initialization, we use standard = 0
    scanf("%lf", &a); // std::cin is replaced by scanf. %lf tells it to expect a double.

    printf(" \nWe can work with that. Please now add a value for the first vector: ");
    double v1 = 0;
    scanf("%lf", &v1);

    printf(" \nOptimal choice indeed. Please add your value for the second vector: ");
    double v2 = 0;
    scanf("%lf", &v2);

    printf("\nOkay, that seems like a computation I could do. \n");

    // Assigning the gathered inputs to the array we passed in
    numbers[0] = a;
    numbers[1] = v1;
    numbers[2] = v2;
}

int getDimension() {
    printf("Could you please specify the dimensionality of the vectors? ");
    int dimension = 0;
    scanf("%d", &dimension); // %d tells scanf to expect an integer

    printf("Perfect, I will start computing now. \n");

    return dimension;
}

// A helper function to handle OS-specific sleep commands
void delay_seconds(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000); // Windows Sleep takes milliseconds
#else
    sleep(seconds);        // POSIX sleep takes seconds
#endif
}

void printResult(double x) {
    // Replacing std::this_thread::sleep_for with our custom delay wrapper
    delay_seconds(2);
    printf("...\n");
    delay_seconds(2);
    printf("...\n");
    delay_seconds(2);

    // In C, standard output is line-buffered. This means "Brrr" won't print
    // immediately without a newline (\n) unless we manually flush the buffer.
    printf("Brrr");
    fflush(stdout); // Force "Brrr" to appear before the delay

    delay_seconds(1);
    printf("rr");
    fflush(stdout);

    delay_seconds(1);
    printf("rr");
    fflush(stdout);

    delay_seconds(1);
    printf("rr");
    fflush(stdout);

    delay_seconds(1);
    printf("rr\n");

    delay_seconds(1);
    printf("...\n");
    delay_seconds(2);

    // Variables are printed by substituting them into format specifiers like %f
    printf("Your result is: %f!\n", x);
}

int main() {
    // 1) Explain User
    printExplanation();

    // 2,3) Obtain user input for scalar, vectors and dimension.
    // Declare an array of 3 doubles, then pass it to getNumbers to be filled
    double user_input[3];
    getNumbers(user_input);
    int dim = getDimension();

    // 4) Perform computation using a for-loop
    double a = user_input[0];

    // C doesn't have std::vector. We allocate dynamic memory manually using malloc.
    // (dim * sizeof(double)) ensures we reserve enough bytes for 'dim' doubles.
    double* v1 = (double*)malloc(dim * sizeof(double));
    double* v2 = (double*)malloc(dim * sizeof(double));
    double* result = (double*)malloc(dim * sizeof(double));

    // Because C doesn't have constructor initialization like v1(dim, value),
    // we must manually populate our arrays with a loop.
    for (int i = 0; i < dim; ++i) {
        v1[i] = user_input[1];
        v2[i] = user_input[2];
    }

    for (int i = 0; i < dim; ++i) {
        result[i] = a * v1[i] + v2[i];
    }

    // 5) Print the result
    printResult(result[0]);

    // C doesn't clean up dynamic memory automatically when variables go out of scope.
    // We MUST use free() to return the memory to the system to avoid memory leaks.
    free(v1);
    free(v2);
    free(result);

    return 0;
}

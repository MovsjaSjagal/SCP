// README
// exercise_2.cpp : This file contains the 'main' function  and helper functions of homework two. 
// 
// The goal of the script is to perform a linear vector operation for given user input values, including vector dimensionality. 
// The values will be provided as numbers (doubles), but the internal operation is done using vector structure.
// 
// The structure of the program is as follows: 
// 1) Explain the user what to do.
// 2) Obtain user input values for scalar and vectors
// 3) Obtain user input values for dimensionality of vectors.
// 4) Perform internal calculation.
// 5) Print result
// 
// 
//

#include <iostream>
#include <vector> // In order to work with vectors of variable size
#include <chrono> // In order to allow for time delay in printResult()
#include <thread> // In order to allow for time delay in printResult()

void printExplanation() {
    std::cout << "We will perfrom a linear vector operation of the type a * v1 + v2, given YOUR input! Each element of each vector will be equal, and the dimension of the vector will need to be speficied by you. \n \n";
}

std::vector<double> getNumbers() {
    std::cout <<  "Please specify the value of the scalar a: ";
    double a{};
    std::cin >> a;


    std::cout << " \nWe can work with that. Please now add a value for the first vector: ";
    double v1{};
    std::cin >> v1;

    std::cout << " \nOptimal choice indeed. Please add your value for the second vector: ";
    double v2{};
    std::cin >> v2;

    std::cout << "\nOkay, that seems like a computation I could do. \n";

    std::vector<double> numbers{ {a, v1, v2} };

    return numbers;
}

int getDimension() {
    std::cout << "Could you please specify the dimensionality of the vectors? ";
    int dimension{};
    std::cin >> dimension;

    std::cout << "Perfect, I will start computing now. \n";

    return dimension;
}

void printResult(double x) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Brrr";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "rr";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "rr";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "rr";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "rr\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Your result is: " << x << "!";
}

int main()
{
    //1) Explain User
    printExplanation();

    //2,3) Obtain user input for scalar, vectors and dimension.
    std::vector<double> user_input{ getNumbers() };
    int dim{ getDimension() };

    //4) Perform computation using a for-loop
    double a{ user_input[0] };
    std::vector<double> v1(dim, user_input[1]);
    std::vector<double> v2(dim, user_input[2]);

    std::vector<double> result(dim, 0);

    for (int i{ 0 }; i < dim; ++i)
        result[i] = a * v1[i] + v2[i];

    //5) Print the result

    printResult(result[0]); // I had some fun with this, but it is completely useless. 
                            //For optimization coment out this line and uncomment the next.
    //std::cout << result[0];

    return 0;
}

#include <iostream>

int main()
{
    // Declare an array of integers with a fixed size
    const int l_s = 5;
    int l_arr[l_s];

    // Initialize the array with values
    for (int l_i = 0; l_i < l_s; l_i++)
    {
        l_arr[l_i] = l_i * 2;
    }

    // Access and print the values in the array
    std::cout << "Elements in the array: ";
    for (int l_i = 0; l_i < l_s; l_i++)
    {
        std::cout << l_arr[l_i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
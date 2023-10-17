#include <iostream>

int main()
{
    int l_r, l_c;
    std::cout << "Enter the number of rows: ";
    std::cin >> l_r;
    std::cout << "Enter the number of columns: ";
    std::cin >> l_c;

    // Allocate memory for a 2D matrix stored as a 1D array
    int *l_dm = new int[l_r * l_c];

    // Check if memory allocation was successful
    if (l_dm == nullptr)
    {
        std::cout << "Memory allocation failed." << std::endl;
        return 1;
    }

    // Input values into the dynamic matrix
    for (int l_i = 0; l_i < l_r; l_i++)
    {
        for (int l_j = 0; l_j < l_c; l_j++)
        {
            int l_index = l_i * l_c + l_j;
            std::cout << "Enter value for element (" << l_i << ", " << l_j << "): ";
            std::cin >> l_dm[l_index];
        }
    }

    // Display the elements of the dynamic matrix
    std::cout << "Elements in the dynamic matrix: ";
    for (int l_i = 0; l_i < l_r * l_c; l_i++)
    {
        std::cout << l_dm[l_i] << " ";
    }

    // Deallocate the memory
    delete[] l_dm;

    return 0;
}
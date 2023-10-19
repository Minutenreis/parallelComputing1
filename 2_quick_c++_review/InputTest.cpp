#include <iostream>

int main()
{
    // Declare a variable to store user input
    float l_age;

    // Prompt the user to enter their age
    std::cout << "Please enter your age: ";
    std::cin >> l_age;

    // Display the entered age
    std::cout << "You entered: " << l_age << " years." << std::endl;

    return 0;
}
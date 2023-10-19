#include <iostream>

int fib(int n);
int fastFib(int n, int n1);

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <n:int>" << std::endl;
        return 1;
    }
    int n = std::stoi(argv[1]);
    std::cout << "fib(" << n << ") = " << fib(n) << std::endl;
    return 0;
}

int fib(int n)
{
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}
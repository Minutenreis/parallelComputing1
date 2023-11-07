#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <algorithm>

double
testDataAccessSpeed(int i_size)
{
    int l_s = i_size;
    double l_scalar = 2.0;

    double *l_A = new double[l_s];
    double *l_B = new double[l_s];
    double *l_C = new double[l_s];

    for (int i = 0; i < l_s; ++i)
    {
        l_A[i] = static_cast<double>(std::rand()) / RAND_MAX;
        l_B[i] = static_cast<double>(std::rand()) / RAND_MAX;
        l_C[i] = 0.0;
    }

    int k, i;

    auto l_start_time = std::chrono::high_resolution_clock::now();
    for (i = 0; i < std::max(l_s, 10000); ++i)
    {
        l_C[i % l_s] = l_A[i % l_s] + l_scalar * l_B[i % l_s];
    }

    auto l_end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> l_duration = l_end_time - l_start_time;

    using namespace std::chrono_literals;
    double l_data_access_speed = 3.0 * std::max(l_s, 10000) * sizeof(double) / (l_duration / 1.0s) / (1024 * 1024 * 1024);

    delete[] l_A;
    delete[] l_B;
    delete[] l_C;

    return l_data_access_speed;
}

int main()
{
    int l_size = 1;

    std::ofstream dataCSV;
    dataCSV.open("memory_bandwidth.csv");
    dataCSV << "Array Size (bytes), Bandwidth (GB/s)" << std::endl;

    for (int i = 0; i < 25; i++)
    {
        dataCSV << l_size << "," << testDataAccessSpeed(l_size) << std::endl;
        l_size *= 2;
    }
    dataCSV.close();

    return 0;
}
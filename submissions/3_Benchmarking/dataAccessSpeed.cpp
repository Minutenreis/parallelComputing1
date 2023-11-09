#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <cmath>

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
    // std::ceil(10000.0 / l_s) to make sure the loop runs for at least 10000 times
    int l_loops = std::ceil(10000.0 / l_s) * 1000;

    auto l_start_time = std::chrono::high_resolution_clock::now();
    for (k = 0; k < l_loops; ++k)
        for (i = 0; i < l_s; ++i)
        {
            l_C[i] = l_A[i] + l_scalar * l_B[i];
        }

    auto l_end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> l_duration = l_end_time - l_start_time;

    double l_data_access_speed = 3.0 * l_s * l_loops * sizeof(double) / (l_duration.count() / 1000) / (1024 * 1024 * 1024);

    delete[] l_A;
    delete[] l_B;
    delete[] l_C;

    return l_data_access_speed;
}

int main()
{
    int l_size[30] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 10000, 16384, 32768, 40000, 50000, 65536, 100000, 131072, 150000, 200000, 262144, 524288, 800000, 1048576, 2097152, 3000000};

    std::ofstream dataCSV;
    dataCSV.open("memory_bandwidth.csv");
    dataCSV << "Array Size (bytes),Bandwidth (GB/s)" << std::endl;

    for (int i = 0; i < 30; i++)
    {
        dataCSV << l_size[i] * sizeof(double) * 3 << "," << testDataAccessSpeed(l_size[i]) << std::endl;
    }
    dataCSV.close();

    return 0;
}
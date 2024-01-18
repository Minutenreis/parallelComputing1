#include <assert.h>
#include <stdio.h>
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>

inline void matmul_mnk(double *i_A,
                       double *i_B,
                       double *io_C,
                       std::size_t i_m,
                       std::size_t i_n,
                       std::size_t i_k)
{

  for (std::size_t l_m = 0; l_m < i_m; l_m++)
    for (std::size_t l_n = 0; l_n < i_n; l_n++)
      for (std::size_t l_k = 0; l_k < i_k; l_k++)
        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

inline void matmul_mkn(double *i_A,
                       double *i_B,
                       double *io_C,
                       std::size_t i_m,
                       std::size_t i_n,
                       std::size_t i_k)
{

  for (std::size_t l_m = 0; l_m < i_m; l_m++)
    for (std::size_t l_k = 0; l_k < i_k; l_k++)
      for (std::size_t l_n = 0; l_n < i_n; l_n++)
        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

inline void matmul_kmn(double *i_A,
                       double *i_B,
                       double *io_C,
                       std::size_t i_m,
                       std::size_t i_n,
                       std::size_t i_k)
{

  for (std::size_t l_k = 0; l_k < i_k; l_k++)
    for (std::size_t l_m = 0; l_m < i_m; l_m++)
      for (std::size_t l_n = 0; l_n < i_n; l_n++)
        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

inline void matmul_knm(double *i_A,
                       double *i_B,
                       double *io_C,
                       std::size_t i_m,
                       std::size_t i_n,
                       std::size_t i_k)
{

  for (std::size_t l_k = 0; l_k < i_k; l_k++)
    for (std::size_t l_n = 0; l_n < i_n; l_n++)
      for (std::size_t l_m = 0; l_m < i_m; l_m++)
        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

inline void matmul_nkm(double *i_A,
                       double *i_B,
                       double *io_C,
                       std::size_t i_m,
                       std::size_t i_n,
                       std::size_t i_k)
{

  for (std::size_t l_n = 0; l_n < i_n; l_n++)
    for (std::size_t l_k = 0; l_k < i_k; l_k++)
      for (std::size_t l_m = 0; l_m < i_m; l_m++)
        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

inline void matmul_nmk(double *i_A,
                       double *i_B,
                       double *io_C,
                       std::size_t i_m,
                       std::size_t i_n,
                       std::size_t i_k)
{

  for (std::size_t l_n = 0; l_n < i_n; l_n++)
    for (std::size_t l_m = 0; l_m < i_m; l_m++)
      for (std::size_t l_k = 0; l_k < i_k; l_k++)
        io_C[l_m * i_n + l_n] += i_A[l_m * i_k + l_k] * i_B[l_k * i_n + l_n];
}

int main(int argc, char *argv[])
{
  std::string path = "performance.csv";
  std::ofstream io_stream;
  io_stream.open(path);

  io_stream << "size, mnk, mkn, kmn, knm, nkm, nmk" << std::endl;
  for (int i = 2; i <= 1024; i *= 2)
  {
    int64_t repeat = 1024 * 1024 * 8 * 10 / (i * i * i);
    if (repeat < 10)
      repeat = 10;

    double *A = new double[i * i];
    double *B = new double[i * i];
    double *C = new double[i * i];

    for (int j = 0; j < i * i; j++)
    {
      A[j] = 1.0;
      B[j] = 1.0;
      C[j] = 0.0;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      matmul_mnk(A, B, C, i, i, i);
    auto end_1 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      matmul_mkn(A, B, C, i, i, i);
    auto end_2 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      matmul_kmn(A, B, C, i, i, i);
    auto end_3 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      matmul_knm(A, B, C, i, i, i);
    auto end_4 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      matmul_nkm(A, B, C, i, i, i);
    auto end_5 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      matmul_nmk(A, B, C, i, i, i);
    auto end_6 = std::chrono::high_resolution_clock::now();

    auto duration_1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_1 - start_time);
    auto duration_2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_2 - end_1);
    auto duration_3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_3 - end_2);
    auto duration_4 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_4 - end_3);
    auto duration_5 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_5 - end_4);
    auto duration_6 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_6 - end_5);

    // Gflops = Double Operation per Nanosecond
    double gflops_1 = 2.0 * i * i * i * repeat / duration_1.count();
    double gflops_2 = 2.0 * i * i * i * repeat / duration_2.count();
    double gflops_3 = 2.0 * i * i * i * repeat / duration_3.count();
    double gflops_4 = 2.0 * i * i * i * repeat / duration_4.count();
    double gflops_5 = 2.0 * i * i * i * repeat / duration_5.count();
    double gflops_6 = 2.0 * i * i * i * repeat / duration_6.count();

    io_stream << i << ", " << gflops_1 << ", " << gflops_2 << ", " << gflops_3 << ", " << gflops_4 << ", " << gflops_5 << ", " << gflops_6 << std::endl;

    delete[] A;
    delete[] B;
    delete[] C;
  }

  return EXIT_SUCCESS;
}
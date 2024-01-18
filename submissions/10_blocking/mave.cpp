#include <stdio.h>
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>

void impl_nonblocked(double *i_A,
                     double *i_b,
                     double *io_c,
                     std::size_t i_k,
                     std::size_t i_m)
{

  for (std::size_t l_m = 0; l_m < i_m; l_m++)
    for (std::size_t l_k = 0; l_k < i_k; l_k++)
      io_c[l_m] += i_A[l_m * i_k + l_k] * i_b[l_k];
}

void impl_blocked_k(double *i_A,
                    double *i_b,
                    double *io_c,
                    std::size_t i_k,
                    std::size_t i_m,
                    std::size_t i_blockSize)
{

  std::size_t l_nBlocks = i_k / i_blockSize;
  for (std::size_t l_bk = 0; l_bk < l_nBlocks; l_bk++)
    for (std::size_t l_m = 0; l_m < i_m; l_m++)
      for (std::size_t l_k = l_bk * i_blockSize; l_k < (l_bk + 1) * i_blockSize; l_k++)
        io_c[l_m] += i_A[l_m * i_k + l_k] * i_b[l_k];
}

void impl_blocked_m(double *i_A,
                    double *i_b,
                    double *io_c,
                    std::size_t i_k,
                    std::size_t i_m,
                    std::size_t i_blockSize)
{

  std::size_t l_nBlocks = i_m / i_blockSize;
  for (std::size_t l_bm = 0; l_bm < l_nBlocks; l_bm++)
    for (std::size_t l_m = l_bm * i_blockSize; l_m < (l_bm + 1) * i_blockSize; l_m++)
      for (std::size_t l_k = 0; l_k < i_k; l_k++)
        io_c[l_m] += i_A[l_m * i_k + l_k] * i_b[l_k];
}

void impl_blocked_mk(double *i_A,
                     double *i_b,
                     double *io_c,
                     std::size_t i_k,
                     std::size_t i_m,
                     std::size_t i_blockSize)
{

  std::size_t l_nBlocks = i_k / i_blockSize;
  for (std::size_t l_bk = 0; l_bk < l_nBlocks; l_bk++)
    for (std::size_t l_bm = 0; l_bm < l_nBlocks; l_bm++)
      for (std::size_t l_m = l_bm * i_blockSize; l_m < (l_bm + 1) * i_blockSize; l_m++)
        for (std::size_t l_k = l_bk * i_blockSize; l_k < (l_bk + 1) * i_blockSize; l_k++)
          io_c[l_m] += i_A[l_m * i_k + l_k] * i_b[l_k];
}

int main(int argc, char *argv[])
{
  std::string path = "performance_blocking.csv";
  std::ofstream io_stream;
  io_stream.open(path);

  io_stream << "size,nonblocking,blocking_k,blocking_m,blocking_mk" << std::endl;

  for (int i = 32; i <= 8192; i *= 2)
  {
    int repeat = 10000 / i;
    if (repeat < 10)
      repeat = 10;

    double *A = new double[i * i];
    double *b = new double[i];
    double *c = new double[i];

    for (int j = 0; j < i * i; j++)
    {
      A[j] = 1.0;
    }
    for (int j = 0; j < i; j++)
    {
      b[j] = 1.0;
      c[j] = 0.0;
    }
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      impl_nonblocked(A, b, c, i, i);
    auto end_time = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      impl_blocked_k(A, b, c, i, i, 4);
    auto end_time2 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      impl_blocked_m(A, b, c, i, i, 4);
    auto end_time3 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
      impl_blocked_mk(A, b, c, i, i, 4);
    auto end_time4 = std::chrono::high_resolution_clock::now();

    auto duration_1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    auto duration_2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - end_time);
    auto duration_3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time3 - end_time2);
    auto duration_4 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time4 - end_time3);

    double gflops_1 = 2.0 * i * i * repeat / duration_1.count();
    double gflops_2 = 2.0 * i * i * repeat / duration_2.count();
    double gflops_3 = 2.0 * i * i * repeat / duration_3.count();
    double gflops_4 = 2.0 * i * i * repeat / duration_4.count();

    io_stream << i << "," << gflops_1 << "," << gflops_2 << "," << gflops_3 << "," << gflops_4 << std::endl;
  }
}

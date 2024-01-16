#include <stdio.h>
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>


void impl_nonblocked_k(double* i_A,
                    double* i_b,
                    double* io_c,
                    std::size_t i_k,
                    std::size_t i_m){

  for( std::size_t l_bk = 0; l_bk < i_k; l_bk++ )
    for( std::size_t l_m = 0; l_m < i_m; l_m++ )
      for( std::size_t l_k = l_bk; l_k < (l_bk+1); l_k++)
        io_c[l_m] += i_A[l_m*i_k + l_k] * i_b[l_k];
}

void impl_blocked_k(double* i_A,
                    double* i_b,
                    double* io_c,
                    std::size_t i_k,
                    std::size_t i_m,
                    std::size_t i_blockSize){

  std::size_t l_nBlocks = i_k / i_blockSize;
  for( std::size_t l_bk = 0; l_bk < l_nBlocks; l_bk++ )
    for( std::size_t l_m = 0; l_m < i_m; l_m++ )
      for( std::size_t l_k = l_bk*i_blockSize; l_k < (l_bk+1)*i_blockSize; l_k++)
        io_c[l_m] += i_A[l_m*i_k + l_k] * i_b[l_k];
}

int main(int argc, char *argv[])
{
  std::string path = "performance_blocking.csv";
  std::ofstream io_stream;
  io_stream.open(path);

  int repeat = 10;
  io_stream << "size, blocking, nonblocking" << std::endl;

    for (int i = 32; i <= 8192; i *= 2)
  {
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
        impl_blocked_k(A, B, C, i, i, i);
    auto end_time = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < repeat; j++)
        impl_nonblocked_k(A, B, C, i, i);
    auto end_time2 = std::chrono::high_resolution_clock::now();

        auto duration_1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time) / repeat;
    auto duration_2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - end_time) / repeat;

        double gflops_1 = 2.0 * i * i * i / duration_1.count();
    double gflops_2 = 2.0 * i * i * i / duration_2.count();
  }
  }




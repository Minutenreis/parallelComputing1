void matmul_mnk(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k ){

  for( std::size_t l_m = 0; l_m < i_m; l_m++ )
    for( std::size_t l_n = 0; l_n < i_n; l_n++ )
      for( std::size_t l_k = 0; l_k < i_k; l_k++ )
        io_C[l_m*i_n + l_n] += i_A[l_m*i_k + l_k] * i_B[l_k*i_n + l_n];
}
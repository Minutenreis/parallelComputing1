#include <cstdlib>
#include <iostream>
#include <chrono>
void rowMajorGEMM(int *i_matrix1, int *i_matrix2, int i_size, int *o_matrix3);
void columnMajorGEMM(int *i_matrix1, int *i_matrix2, int i_size, int *o_matrix3);

int main()
{
    int l_size = 1000;

    // Allocate memory for a 2D matrix stored as a 1D array
    int *l_matrix1 = new int[l_size * l_size];
    int *l_matrix2 = new int[l_size * l_size];
    int *l_matrix3 = new int[l_size * l_size];

    // randomize the values in the matrices
    for (int l_i = 0; l_i < l_size * l_size; l_i++)
    {
        l_matrix1[l_i] = rand() % 100;
        l_matrix2[l_i] = rand() % 100;
    }

    // matrix1 * matrix2 = matrix3

    // Row major matrix multiplication
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    rowMajorGEMM(l_matrix1, l_matrix2, l_size, l_matrix3);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "rowMajor took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

    // Column major matrix multiplication
    begin = std::chrono::steady_clock::now();
    columnMajorGEMM(l_matrix1, l_matrix2, l_size, l_matrix3);
    end = std::chrono::steady_clock::now();
    std::cout << "columnMajor took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

    return 0;
}

/**
 * @brief Row major matrix multiplication of 2 quadratic matrices
 * @param i_matrix1 Pointer to the first matrix
 * @param i_matrix2 Pointer to the second matrix
 * @param i_size Size of the matrices
 * @param o_matrix3 Pointer to the result matrix
 */
void rowMajorGEMM(int *i_matrix1, int *i_matrix2, int i_size, int *o_matrix3)
{
    for (int i = 0; i < i_size; i++)
        for (int j = 0; j < i_size; j++)
            for (int k = 0; k < i_size; k++)
            {
                o_matrix3[i * i_size + j] += i_matrix1[i * i_size + k] * i_matrix2[k * i_size + j];
            }
}

/**
 * @brief Column major matrix multiplication of 2 quadratic matrices
 * @param i_matrix1 Pointer to the first matrix
 * @param i_matrix2 Pointer to the second matrix
 * @param i_size Size of the matrices
 * @param o_matrix3 Pointer to the result matrix
 */
void columnMajorGEMM(int *i_matrix1, int *i_matrix2, int i_size, int *o_matrix3)
{
    for (int j = 0; j < i_size; j++)
        for (int i = 0; i < i_size; i++)
            for (int k = 0; k < i_size; k++)
            {
                o_matrix3[i * i_size + j] += i_matrix1[i * i_size + k] * i_matrix2[k * i_size + j];
            }
}
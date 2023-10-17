#include <cstdlib>
#include <iostream>
void rowMajorGEMM(int *i_matrix1, int *i_matrix2, int *o_matrix3, int i_size);

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
    rowMajorGEMM(l_matrix1, l_matrix2, l_matrix3, l_size);

    for (int i = 0; i < l_size * l_size; i++)
    {
        std::cout << l_matrix3[i] << " ";
        if (i % l_size == 0)
            std::cout << std::endl;
    }

    return 0;
}

/**
 * @brief Row major matrix multiplication of 2 quadratic matrices
 * @param i_matrix1 Pointer to the first matrix
 * @param i_matrix2 Pointer to the second matrix
 * @param o_matrix3 Pointer to the result matrix
 * @param i_size Size of the matrices
 */
void rowMajorGEMM(int *i_matrix1, int *i_matrix2, int *o_matrix3, int i_size)
{
    for (int i = 0; i < i_size; i++)
        for (int j = 0; j < i_size; i++)
            for (int k = 0; k < i_size; i++)
            {
                o_matrix3[i * i_size + j] += i_matrix1[i * i_size + k] * i_matrix2[k * i_size + j];
            }
}

/**
 * @brief Column major matrix multiplication of 2 quadratic matrices
 * @param i_matrix1 Pointer to the first matrix
 * @param i_matrix2 Pointer to the second matrix
 * @param o_matrix3 Pointer to the result matrix
 * @param i_size Size of the matrices
 */
void columnMajorGEMM(int *i_matrix1, int *i_matrix2, int *o_matrix3, int i_size)
{
}
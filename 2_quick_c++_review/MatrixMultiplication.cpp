#include <cstdlib>
#include <iostream>
#include <chrono>
void rowMajorGEMM(int *i_matrix1, int *i_matrix2, int i_size, int *o_matrix3);
void columnMajorGEMM(int *i_matrix1, int *i_matrix2, int i_size, int *o_matrix3);
bool getTimeOnce(int i_size, int timeRowMajor[], int timeColumnMajor[], int iteration);

int main()
{
    // initiate start conditions (size of the array, number of tries)
    int l_size = 1000;
    int l_iterations = 3;
    int *l_timeRowMajor = new int[l_iterations];
    int *l_timeColumnMajor = new int[l_iterations];
    for (int i = 0; i < l_iterations; ++i)
    {
        bool success = getTimeOnce(l_size, l_timeRowMajor, l_timeColumnMajor, i);
        if (!success)
        {
            std::cout << "Error at iteration " << i << std::endl;
            return 1;
        }
    }

    int l_timeRowMajorSum = 0;
    int l_timeColumnMajorSum = 0;
    for (int i = 0; i < l_iterations; ++i)
    {
        l_timeRowMajorSum += l_timeRowMajor[i];
        l_timeColumnMajorSum += l_timeColumnMajor[i];
    }
    std::cout << "Average time for row major multiplication: " << l_timeRowMajorSum / l_iterations << "ms" << std::endl;
    std::cout << "Average time for column major multiplication: " << l_timeColumnMajorSum / l_iterations << "ms" << std::endl;

    delete[] l_timeRowMajor;
    delete[] l_timeColumnMajor;
    return 0;
}

/**
 * @brief test the time of matrix multiplication once and saves it at the iteration
 * index on timeRowMjaor and timeColumnMajor respectively
 *
 * @param i_size length of the quadratic matrix
 * @param timeRowMajor array to save the time of row major multiplication
 * @param timeColumnMajor array to save the time of column major multiplication
 * @param iteration index of the iteration
 * @return bool
 */
bool getTimeOnce(int i_size, int timeRowMajor[], int timeColumnMajor[], int iteration)
{
    // Allocate memory for a 2D matrix stored as a 1D array
    int *l_matrix1 = new int[i_size * i_size];
    int *l_matrix2 = new int[i_size * i_size];
    int *l_matrixRowMajor = new int[i_size * i_size];
    int *l_matrixColumnMajor = new int[i_size * i_size];

    // randomize the values in the matrices
    for (int l_i = 0; l_i < i_size * i_size; ++l_i)
    {
        l_matrix1[l_i] = rand() % 100;
        l_matrix2[l_i] = rand() % 100;
        l_matrixRowMajor[l_i] = 0;
    }

    // matrix1 * matrix2 = matrix3

    // Row major matrix multiplication
    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    rowMajorGEMM(l_matrix1, l_matrix2, i_size, l_matrixRowMajor);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    timeRowMajor[iteration] = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    // Column major matrix multiplication
    begin = std::chrono::high_resolution_clock::now();
    columnMajorGEMM(l_matrix1, l_matrix2, i_size, l_matrixColumnMajor);
    end = std::chrono::high_resolution_clock::now();
    timeColumnMajor[iteration] = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    delete[] l_matrix1;
    delete[] l_matrix2;

    for (int i = 0; i < i_size * i_size; ++i)
    {
        if (l_matrixRowMajor[i] != l_matrixColumnMajor[i])
        {
            std::cout << "Error at index " << i << std::endl;
            delete[] l_matrixRowMajor;
            delete[] l_matrixColumnMajor;
            return false;
        }
    }
    delete[] l_matrixRowMajor;
    delete[] l_matrixColumnMajor;
    return true;
}

/**
 * @brief Row major matrix multiplication of 2 quadratic matrices
 * @param i_matrix1 Pointer to the first matrix
 * @param i_matrix2 Pointer to the second matrix
 * @param i_size Size of the matrices
 * @param o_matrix3 Pointer to the result matrix
 */
void rowMajorGEMM(int i_matrix1[], int i_matrix2[], int i_size, int o_matrix3[])
{
    int i, j, k;
    for (i = 0; i < i_size; ++i)
        for (j = 0; j < i_size; ++j)
            for (k = 0; k < i_size; ++k)
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
    int i, j, k;
    for (j = 0; j < i_size; ++j)
        for (i = 0; i < i_size; ++i)
            for (k = 0; k < i_size; ++k)
            {
                o_matrix3[i * i_size + j] += i_matrix1[i * i_size + k] * i_matrix2[k * i_size + j];
            }
}
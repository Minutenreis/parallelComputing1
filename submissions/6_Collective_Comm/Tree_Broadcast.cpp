#include <mpi.h>
#include <iostream>
#include <cassert>
#include <chrono>

bool success(int status);
void tree_broadcast(double *values, int N, int i_rank, int i_comm_size);

int main(int argc, char *argv[])
{
    int l_err = MPI_Init(&argc, &argv);
    assert(success(l_err));

    if (argc != 2)
    {
        std::cerr << "Usage: Tree_Broadcast.out N" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    int N = atoi(argv[1]);

    int l_rank;
    int l_comm_size;

    l_err = MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
    assert(success(l_err));
    l_err = MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);
    assert(success(l_err));

    if (l_comm_size < 3)
    {
        std::cerr << "Error: at least 3 processes required" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    double *values = new double[N];

    for (int i = 0; i < N; i++)
    {
        srand((unsigned int)time(NULL) * i);
        values[i] = (double)rand() / RAND_MAX;
    }

    auto start = std::chrono::high_resolution_clock::now();
    tree_broadcast(values, N, l_rank, l_comm_size);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (l_rank == l_comm_size / 2)
    {
        std::cout << "tree_broadcast Number of Processes: " << l_comm_size << " Number of Elements: " << N << " Time: " << elapsed.count() << " seconds" << std::endl;
    }

    start = std::chrono::high_resolution_clock::now();
    MPI_Bcast(values, N, MPI_DOUBLE, l_comm_size / 2, MPI_COMM_WORLD);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    if (l_rank == l_comm_size / 2)
    {
        std::cout << "MPI_Bcast      Number of Processes: " << l_comm_size << " Number of Elements: " << N << " Time: " << elapsed.count() << " seconds" << std::endl;
    }

    delete[] values;

    l_err = MPI_Finalize();
    assert(success(l_err));

    return EXIT_SUCCESS;
}

bool success(int status)
{
    return status == MPI_SUCCESS;
}

void tree_broadcast(double *values, int N, int i_rank, int i_comm_size)
{
    int root = i_comm_size / 2;
    if (i_rank == root)
    {
        MPI_Send(values, N, MPI_DOUBLE, root - 1, 0, MPI_COMM_WORLD);
        MPI_Send(values, N, MPI_DOUBLE, root + 1, 0, MPI_COMM_WORLD);
    }
    else if (i_rank < root)
    {
        MPI_Recv(values, N, MPI_DOUBLE, i_rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // std::cout << "Process " << i_rank << " received message from " << i_rank + 1 << " with values ";
        // for (int i = 0; i < N; i++)
        // {
        //     std::cout << values[i] << " ";
        // }
        // std::cout << std::endl;
        if (i_rank > 0)
            MPI_Send(values, N, MPI_DOUBLE, i_rank - 1, 0, MPI_COMM_WORLD);
    }
    else // i_rank > root
    {
        MPI_Recv(values, N, MPI_DOUBLE, i_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // std::cout << "Process " << i_rank << " received message from " << i_rank - 1 << " with values ";
        // for (int i = 0; i < N; i++)
        // {
        //     std::cout << values[i] << " ";
        // }
        // std::cout << std::endl;
        if (i_rank < i_comm_size - 1)
            MPI_Send(values, N, MPI_DOUBLE, i_rank + 1, 0, MPI_COMM_WORLD);
    }
}
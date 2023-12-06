#include <mpi.h>
#include <iostream>
#include <cassert>

bool success(int status);
int main(int argc, char *argv[])
{
    int l_err = MPI_Init(&argc, &argv);
    assert(success(l_err));

    int l_rank;
    int l_comm_size;

    l_err = MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
    assert(success(l_err));
    l_err = MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);
    assert(success(l_err));

    MPI_Win l_win;
    int l_data = 100;

    MPI_Win_create(&l_data, 1 * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);

    MPI_Win_fence(MPI_MODE_NOPRECEDE, l_win);

    if (l_rank != 0)
    {
        MPI_Get(&l_data, 1, MPI_INT, 0, 0, 1, MPI_INT, l_win);
    }

    MPI_Win_fence(0, l_win);

    if (l_rank == 1)
    {
        std::cout << "l_data of rank 1" << l_data << std::endl;
    }

    l_err = MPI_Finalize();
    assert(success(l_err));

    return EXIT_SUCCESS;
}

bool success(int status)
{
    return status == MPI_SUCCESS;
}
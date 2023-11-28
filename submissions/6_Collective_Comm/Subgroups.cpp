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

    if (l_comm_size % 4 != 0)
    {
        std::cerr << "Error: Processes need to be divisible by 4" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    MPI_Comm MPI_COMM_SPLIT;
    int color = l_rank * 4 / l_comm_size;

    l_err = MPI_Comm_split(MPI_COMM_WORLD, color, l_rank, &MPI_COMM_SPLIT);
    assert(success(l_err));

    int l_rank_split;
    int l_comm_size_split;
    l_err = MPI_Comm_rank(MPI_COMM_SPLIT, &l_rank_split);
    assert(success(l_err));

    l_err = MPI_Comm_size(MPI_COMM_SPLIT, &l_comm_size_split);
    assert(success(l_err));

    std::cout << "Old Rank: " << l_rank << "\tNew Rank: " << l_rank_split << "\tColor: " << color << "\tOld Communicator: MPI_COMM_WORLD\tNew Communicator: MPI_COMM_SPLIT" << std::endl;

    l_err = MPI_Comm_free(&MPI_COMM_SPLIT);
    assert(success(l_err));

    l_err = MPI_Finalize();
    assert(success(l_err));

    return EXIT_SUCCESS;
}

bool success(int status)
{
    return status == MPI_SUCCESS;
}

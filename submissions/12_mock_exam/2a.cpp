#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank;
    int data[5];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        for (int i = 0; i < 5; ++i)
        {
            data[i] = i + 1;
        }

        MPI_Bcast(data, 5, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Bcast(data, 5, MPI_INT, 0, MPI_COMM_WORLD);
    }

    std::cout << "Rank " << rank << " Werte in data: " << data[0] << ", " << data[1] << ", " << data[2] << ", " << data[3] << ", " << data[4] << std::endl;

    MPI_Finalize();

    return 0;
}
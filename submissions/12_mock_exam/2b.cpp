#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank;
    int recv_data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        int data[] = {1, 2, 3, 4, 5};
        MPI_Scatter(data, 1, MPI_INT, MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Scatter(nullptr, 0, MPI_DATATYPE_NULL, &recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    std::cout << "Rank " << rank << " Werte in recv_data: " << recv_data << std::endl;

    MPI_Finalize();

    return 0;
}

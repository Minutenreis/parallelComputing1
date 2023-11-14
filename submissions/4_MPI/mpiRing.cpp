#include <iostream>
#include <mpi.h>
#include <cassert>

bool success(int a);
void sendValue(int value, int receiver_rank, int own_rank);
void readValue(int &value, int sender_rank, int own_rank);

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

    if (l_comm_size < 2)
    {
        std::cerr << "Error: at least 2 processes required" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int l_value = 10;

    if (l_rank == 0)
    {
        sendValue(l_value, l_rank + 1, l_rank);
        readValue(l_value, l_comm_size - 1, l_rank);
    }
    else
    {
        readValue(l_value, l_rank - 1, l_rank);
        if (l_rank != l_comm_size - 1)
            sendValue(l_value, (l_rank + 1) % l_comm_size, l_rank);
        else
            sendValue(l_value + l_rank, 0, l_rank);
    }

    l_err = MPI_Finalize();
    assert(success(l_err));

    return EXIT_SUCCESS;
}

bool success(int a)
{
    return a == MPI_SUCCESS;
}

void sendValue(int value, int receiver_rank, int own_rank)
{
    int l_err = MPI_Send(&value, 1, MPI_INT, receiver_rank, 0, MPI_COMM_WORLD);
    assert(success(l_err));
    std::cout << "Process " << own_rank << " sent value " << value << " to process " << receiver_rank << std::endl;
}

void readValue(int &value, int sender_rank, int own_rank)
{
    MPI_Status status;
    int l_err = MPI_Recv(&value, 1, MPI_INT, sender_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    assert(success(l_err));
    std::cout << "Process " << own_rank << " received value " << value << " from process " << sender_rank << std::endl;
}
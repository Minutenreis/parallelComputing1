#include <mpi.h>
#include <assert.h>
#include <stdio.h>

bool success(int err)
{
    return err == MPI_SUCCESS;
}

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

    int data[2] = {0, 0};
    MPI_Win win;

    if (l_rank == 0)
    {
        data[0] = 100; // some data to send
    }
    else if (l_rank == 1)
    {
        data[0] = 200; // some data to send
    }

    // Create a window for one-sided communication
    l_err = MPI_Win_create(data, 2 * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    assert(success(l_err));

    // Start one-sided communication
    l_err = MPI_Win_fence(0, win);
    assert(success(l_err));

    // Both processes put their data into the window of the other process
    l_err = MPI_Put(&data[0], 1, MPI_INT, (l_rank + 1) % 2, 1, 1, MPI_INT, win);
    assert(success(l_err));

    // Complete one-sided communication
    l_err = MPI_Win_fence(0, win);
    assert(success(l_err));

    printf("Process %d received data: %d\n", l_rank, data[1]);

    l_err = MPI_Win_free(&win);
    assert(success(l_err));

    l_err = MPI_Finalize();
    assert(success(l_err));

    return 0;
}
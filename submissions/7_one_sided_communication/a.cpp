#include <mpi.h>
#include <assert.h>
#include <stdio.h>

bool success(int status)
{
    return status == MPI_SUCCESS;
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

    int data;
    MPI_Win win;

    if (l_rank == 1)
    {
        data = 100; // some data to send
    }

    // Create a window for one-sided communication
    l_err = MPI_Win_create(&data, sizeof(int), 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    assert(success(l_err));

    // Start one-sided communication
    l_err = MPI_Win_fence(0, win);
    assert(success(l_err));

    if (l_rank == 1)
    {
        // Process 1 puts its data into the window of process 0
        l_err = MPI_Put(&data, 1, MPI_INT, 0, 0, 1, MPI_INT, win);
        assert(success(l_err));
    }

    // Complete one-sided communication
    l_err = MPI_Win_fence(0, win);
    assert(success(l_err));

    if (l_rank == 0)
    {
        // Process 0 now has the data from process 1
        printf("Received data: %d\n", data);
    }

    l_err = MPI_Win_free(&win);
    assert(success(l_err));

    l_err = MPI_Finalize();
    assert(success(l_err));

    return 0;
}
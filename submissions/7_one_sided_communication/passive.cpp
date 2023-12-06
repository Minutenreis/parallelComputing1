#include <mpi.h>
#include <assert.h>

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

    // Create a buffer of size equal to the number of processes for process 0 and size 1 for all other processes
    double *buf = new double[l_rank == 0 ? l_comm_size * sizeof(double) : sizeof(double)];
    buf[0] = l_rank * 10.0; // Initialize the buffer values to rank * 10

    MPI_Win win;
    // Create a window for one-sided communication
    l_err = MPI_Win_create(buf, l_rank == 0 ? l_comm_size * sizeof(double) : sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    assert(success(l_err));

    // Passive target synchronization
    if (l_rank != 0)
    {
        l_err = MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
        assert(success(l_err));
        // All other processes put their data into the window of process 0
        l_err = MPI_Put(buf, 1, MPI_DOUBLE, 0, l_rank, 1, MPI_DOUBLE, win);
        assert(success(l_err));
        l_err = MPI_Win_unlock(0, win);
        assert(success(l_err));
    }

    if (l_rank == 0)
    {
        for (int i = 0; i < l_comm_size; i++)
        {
            printf("buf[%d] = %.1f\n", i, buf[i]);
        }
    }

    delete[] buf;
    l_err = MPI_Win_free(&win);
    assert(success(l_err));

    l_err = MPI_Finalize();
    assert(success(l_err));

    return 0;
}
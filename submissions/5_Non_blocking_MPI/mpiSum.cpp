#include <iostream>
#include <mpi.h>
#include <cassert>
#include <ctime>

bool success(int a);
void my_custom_reduction(int i_rank, int i_comm_size);

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

    my_custom_reduction(l_rank, l_comm_size);

    l_err = MPI_Finalize();
    assert(success(l_err));

    return EXIT_SUCCESS;
}

bool success(int a)
{
    return a == MPI_SUCCESS;
}

void my_custom_reduction(int i_rank, int i_comm_size)
{
    if (i_rank == 0)
    {
        double *values = new double[i_comm_size - 1];
        MPI_Request *requests = new MPI_Request[i_comm_size - 1];
        for (int i = 0; i < i_comm_size - 1; i++)
        {
            MPI_Irecv(&values[i], 1, MPI_DOUBLE, i + 1, 0, MPI_COMM_WORLD, &requests[i]);
        }

        int flag;
        while (true)
        {
            MPI_Testall(i_comm_size - 1, requests, &flag, MPI_STATUS_IGNORE);
            if (flag)
                break;
        }
        MPI_Waitall(i_comm_size - 1, requests, MPI_STATUSES_IGNORE);
        double sum = 0;
        std::cout << "Array of values: ";
        for (int i = 0; i < i_comm_size - 1; i++)
        {
            sum += values[i];
            std::cout << values[i] << " ";
        }
        std::cout << std::endl
                  << "Sum: " << sum << std::endl;
        delete[] values;
        delete[] requests;
    }
    else
    {
        srand((unsigned int)time(NULL) * i_rank);
        double val = (double)rand() / RAND_MAX;
        std::cout << "Process " << i_rank << " sent value " << val << " to process 0" << std::endl;
        MPI_Request request;
        MPI_Isend(&val, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &request);
        int flag;
        while (true)
        {
            MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
            if (flag)
                break;
        }
    }
}
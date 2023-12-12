#include <mpi.h>
#include <assert.h>
#include <stdio.h>
#include <random>
#include <iostream>

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

    long long int total_samples = 50000000;
    long long int samples_per_process = (total_samples / l_comm_size) + 1;
    long long int inside_circle = 0;

    std::random_device rd;     // Will be used to obtain a seed for the random number engine
    std::mt19937_64 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double start_time = MPI_Wtime();

    for (long long int i = 0; i < samples_per_process; ++i)
    {
        double x = dis(gen);
        double y = dis(gen);
        double distance_squared = x * x + y * y;
        if (distance_squared <= 1)
        {
            inside_circle++;
        }
    }

    long long int total_inside_circle = 0;
    l_err = MPI_Reduce(&inside_circle, &total_inside_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    assert(success(l_err));

    if (l_rank == 0)
    {
        double pi = 4.0 * total_inside_circle / (samples_per_process * l_comm_size);
        double error = fabs(pi - M_PI);
        double time_taken = MPI_Wtime() - start_time;
        std::cout << "Computed pi: " << pi << ", Error: " << error << ", Time taken: " << time_taken << " seconds\n";
    }

    l_err = MPI_Finalize();
    assert(success(l_err));

    return EXIT_SUCCESS;
}
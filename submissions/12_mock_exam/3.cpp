#include <iostream>
#include <mpi.h> //i.

// Hinweis: die Funktion wurde von int[] auf double[] geändert, da sonst die Aufgabe keinen Sinn ergibt
// b) multipliziert 2 Vektoren i_a und i_b elementweise und speichert das Ergebnis in o_c (quasi fmul?)
void myFunction(double o_c[],
                const double i_a[],
                const double i_b[],
                int i_n)
{
    for (int i = 0; i < i_n; ++i)
    {
        o_c[i] = i_a[i] * i_b[i];
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv); // ii.

    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int l_total_n = 100000;
    int l_n_per_rank = l_total_n / world_size; // iii.
    const int root = 0;

    double *l_total_a = new double[l_total_n];
    double *l_total_b = new double[l_total_n];
    double *l_total_c = new double[l_total_n];

    double *l_a = new double[l_n_per_rank];
    double *l_b = new double[l_n_per_rank];
    double *l_c = new double[l_n_per_rank];

    if (rank == root)
    {
        for (int i = 0; i < l_total_n; ++i)
        {
            l_total_a[i] = static_cast<double>(i) / 2.0;
            l_total_b[i] = static_cast<double>(i) / 2.0;
        }
    }

    MPI_Scatter(l_total_a, l_n_per_rank, MPI_DOUBLE, l_a, l_n_per_rank, MPI_DOUBLE, root, MPI_COMM_WORLD); // iv.
    MPI_Scatter(l_total_b, l_n_per_rank, MPI_DOUBLE, l_b, l_n_per_rank, MPI_DOUBLE, root, MPI_COMM_WORLD); // v.

    myFunction(l_c, l_a, l_b, l_n_per_rank); // vi.

    MPI_Gather(l_c, l_n_per_rank, MPI_DOUBLE, l_total_c, l_n_per_rank, MPI_DOUBLE, root, MPI_COMM_WORLD); // vii.

    if (rank == root)
    {
        // verify
        for (int i = 0; i < l_total_n; ++i)
        {
            if (l_total_c[i] != l_total_a[i] * l_total_b[i])
            {
                std::cout << "Error at index " << i << std::endl;
                break;
            }
        }
    }

    std::cout << "Finished" << std::endl;

    delete[] l_total_a; // viii.
    delete[] l_total_b;
    delete[] l_total_c;

    delete[] l_a;
    delete[] l_b;
    delete[] l_c;

    MPI_Finalize();

    return 0;
}

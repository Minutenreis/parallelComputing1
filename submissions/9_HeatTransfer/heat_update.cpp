#include <mpi.h>
#include "heat.h"
#include <assert.h>
#include <iostream>

// Include header files if necessary

void start_halo_exchange(Field *temperature, ParallelData *parallel)
{

    // This function should initiate the halo exchange to communicate boundary data between neighboring processes.

    // Width for accessing and navigating through the temperature field
    int width = temperature->ny + 2;

    // (up)
    // Communication 1: Send data to the upper neighbor and receive from the lower neighbor
    // This exchanges the ghost cells in the top row of the local temperature field
    MPI_Isend(&temperature->data[idx(0, 1, width)], 1, parallel->columntype, parallel->nup, 0, parallel->comm, &parallel->requests[0]);
    MPI_Irecv(&temperature->data[idx(0, 0, width)], 1, parallel->columntype, parallel->nup, MPI_ANY_TAG, parallel->comm, &parallel->requests[4]);

    // (down)
    // Communication 2: Send data to the lower neighbor and receive from the upper neighbor
    // This exchanges the ghost cells in the bottom row of the local temperature field
    MPI_Isend(&temperature->data[idx(0, temperature->ny, width)], 1, parallel->columntype, parallel->ndown, 0, parallel->comm, &parallel->requests[1]);
    MPI_Irecv(&temperature->data[idx(0, temperature->ny + 1, width)], 1, parallel->columntype, parallel->ndown, MPI_ANY_TAG, parallel->comm, &parallel->requests[5]);

    // (left)
    // Communication 3: Send data to the left neighbor and receive from the right neighbor
    // This exchanges the ghost cells in the leftmost column of the local temperature field
    MPI_Isend(&temperature->data[idx(1, 0, width)], 1, parallel->rowtype, parallel->nleft, 0, parallel->comm, &parallel->requests[2]);
    MPI_Irecv(&temperature->data[idx(0, 0, width)], 1, parallel->rowtype, parallel->nleft, MPI_ANY_TAG, parallel->comm, &parallel->requests[6]);

    // (right)
    // Communication 4: Send data to the right neighbor and receive from the left neighbor
    // This exchanges the ghost cells in the rightmost column of the local temperature field
    MPI_Isend(&temperature->data[idx(temperature->nx, 0, width)], 1, parallel->rowtype, parallel->nright, 0, parallel->comm, &parallel->requests[3]);
    MPI_Irecv(&temperature->data[idx(temperature->nx + 1, 0, width)], 1, parallel->rowtype, parallel->nright, MPI_ANY_TAG, parallel->comm, &parallel->requests[7]);
}

// wait that all the sends are done
void complete_halo_exchange(ParallelData *parallel)
{
    // Wait for the completion of non-blocking communication requests related to halo exchange
    MPI_Waitall(8, parallel->requests, MPI_STATUSES_IGNORE);
}

double stencil(double center, double up, double down, double left, double right, double a, double dt, double dx2, double dy2)
{
    return center + a * dt * (right - 2.0 * center + left) / dx2 + a * dt * (up - 2.0 * center + down) / dy2;
}

void update_interior_temperature(Field *curr, Field *prev, double a, double dt)
{

    // This function should update the interior temperature field based on the five-point stencil.

    // Indices for center, up, down, left, right
    // These indices are used for accessing neighboring grid points during the update.
    int ic, iu, id, il, ir;

    // Width of the grid (number of columns)
    // The width is used to calculate the indices and navigate through the temperature field.
    int width = curr->ny + 2;

    // Parameters for the update
    // a: Thermal diffusivity
    // dt: Time step size

    // Determine the temperature field at the next time step
    // As fixed boundary conditions are applied, the outermost grid points are not updated.
    double dx2 = prev->dx * prev->dx;
    double dy2 = prev->dy * prev->dy;

    // Loop over the interior grid points for the update
    // Update the temperature using the five-point stencil
    for (int i = 2; i < curr->nx - 1; ++i)
    {
        for (int j = 2; j < curr->ny - 1; ++j)
        {
            ic = idx(i, j, width);
            iu = idx(i + 1, j, width);
            id = idx(i - 1, j, width);
            ir = idx(i, j + 1, width);
            il = idx(i, j - 1, width);

            // Apply the five-point stencil to update the temperature at the left and right borders.
            curr->data[ic] = stencil(prev->data[ic], prev->data[iu], prev->data[id], prev->data[il], prev->data[ir], a, dt, dx2, dy2);
        }
    }
}

void update_boundary_temperature(Field *curr, Field *prev, ParallelData *par, double a, double dt)
{
    int i, j;
    int ic, iu, id, il, ir; // Indices for center, up, down, left, right
    int width;
    width = curr->ny + 2;
    double dx2, dy2;

    // Determine the temperature field at the next time step. As fixed boundary conditions are applied, the outermost grid points are not updated.
    dx2 = prev->dx * prev->dx;
    dy2 = prev->dy * prev->dy;

    // Update the left and right borders
    i = 1;
    for (j = 1; j < curr->ny + 1; j++)
    {
        ic = idx(i, j, width);
        iu = idx(i + 1, j, width);
        id = idx(i - 1, j, width);
        ir = idx(i, j + 1, width);
        il = idx(i, j - 1, width);

        // Apply the five-point stencil to update the temperature at the left and right borders.
        curr->data[ic] = stencil(prev->data[ic], prev->data[iu], prev->data[id], prev->data[il], prev->data[ir], a, dt, dx2, dy2);
    }

    i = curr->nx;
    for (j = 1; j < curr->ny + 1; j++)
    {
        ic = idx(i, j, width);
        iu = idx(i + 1, j, width);
        id = idx(i - 1, j, width);
        ir = idx(i, j + 1, width);
        il = idx(i, j - 1, width);

        // Apply the five-point stencil to update the temperature at the left and right borders.
        curr->data[ic] = stencil(prev->data[ic], prev->data[iu], prev->data[id], prev->data[il], prev->data[ir], a, dt, dx2, dy2);
    }

    // Update the upper and lower borders
    j = 1;
    for (i = 1; i < curr->nx + 1; i++)
    {
        ic = idx(i, j, width);
        iu = idx(i + 1, j, width);
        id = idx(i - 1, j, width);
        ir = idx(i, j + 1, width);
        il = idx(i, j - 1, width);

        // Apply the five-point stencil to update the temperature at the upper and lower borders.
        curr->data[ic] = stencil(prev->data[ic], prev->data[iu], prev->data[id], prev->data[il], prev->data[ir], a, dt, dx2, dy2);
    }

    // Update the lower and upper borders
    j = curr->ny;
    for (i = 1; i < curr->nx + 1; i++)
    {
        ic = idx(i, j, width);
        iu = idx(i + 1, j, width);
        id = idx(i - 1, j, width);
        ir = idx(i, j + 1, width);
        il = idx(i, j - 1, width);

        // Apply the five-point stencil to update the temperature at the upper and lower borders.
        curr->data[ic] = stencil(prev->data[ic], prev->data[iu], prev->data[id], prev->data[il], prev->data[ir], a, dt, dx2, dy2);
    }
}
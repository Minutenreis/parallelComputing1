#!/bin/bash
#SBATCH -J heat_mpi_job            # Job name
#SBATCH -o heat_mpi_output_%j.out  # Output file name
#SBATCH -e heat_mpi_error_%j.err   # Error file name
#SBATCH --partition=s_hadoop,s_standard
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --cpus-per-task=72

# Purge all currently loaded modules to start with a clean environment
module purge

# Load necessary modules
module load compiler/gcc/11.2.0
module load mpi/openmpi/4.1.2-gcc-10.2.0

# Compile the MPI program
mpic++ -Ofast -march=native -mtune=native -Wall -c main.cpp pngsaver.cpp heat_init.cpp heat_update.cpp heat_io.cpp

# Link the compiled objects into an executable

# -Ofast: Optimization level fast
# -Wall: Enable most warning messages
# -o: Output file name
# -lpng: Link against the libpng library
# -lm: Link against the math library
# -march=native: Enable all instruction subsets supported by the local machine
# -mtune=native: Tune the performance to the local machine
mpic++ -Ofast -march=native -mtune=native -Wall -o heat_mpi main.o pngsaver.o heat_init.o heat_update.o heat_io.o -lpng -lm

# Run the MPI program using mpirun
# -np 8: Number of MPI tasks
# --oversubscribe: Allow more processes than available cores
for i in 1 2 4 8 16 32 64 128
do
    mpirun --oversubscribe -n $i heat_mpi
    rm -f *.png # Remove old PNG files
    rm -f HEAT.dat # Remove old DAT files
done

# Clean up object files after the run
rm -f *.o

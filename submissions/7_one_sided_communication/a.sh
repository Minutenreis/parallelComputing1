#!/bin/bash
#SBATCH --job-name=a
#SBATCH --output=a.output
#SBATCH --error=a.err
#SBATCH --partition=s_hadoop,s_standard
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --cpus-per-task=2

# Load any necessary modules (if needed)
# module load mymodule
module load mpi/openmpi/4.1.2-gcc-10.2.0


# Enter your executable commands here
# Execute the compiled program
date
mpicxx -o a.o a.cpp
mpirun -np 2 -oversubscribe ./a.o
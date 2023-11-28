#!/bin/bash
#SBATCH --job-name=mpiTreeBroadcast
#SBATCH --output=mpiTreeBroadcast.output
#SBATCH --error=mpiTreeBroadcast.err
#SBATCH --partition=s_hadoop,s_standard
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --cpus-per-task=72

# Load any necessary modules (if needed)
# module load mymodule
module load mpi/openmpi/4.1.2-gcc-10.2.0


# Enter your executable commands here
# Execute the compiled program
date
mpicxx -o Tree_Broadcast.o Tree_Broadcast.cpp
mpirun -np 100 -oversubscribe ./Tree_Broadcast.o 100000
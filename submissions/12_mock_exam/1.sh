#!/bin/bash
#SBATCH --partition=s_hadoop
#SBATCH --nodes=10
#SBATCH --cpus-per-task=48
#SBATCH --job-name=quantum_job
#SBATCH --time=10:00:00

# meiner Meinung nach reicht `module load openmpi/4.0.3`
module load mpi/openmpi/4.0.3-gcc-10.2.0

mpicxx -o quantum_sim quantum_sim.cpp
mpirun -np 480 ./quantum_sim
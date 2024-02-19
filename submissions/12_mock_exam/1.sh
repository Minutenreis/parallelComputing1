#!/bin/bash
#SBATCH --partition=s_hadoop
#SBATCH --nodes=2
#SBATCH --ntasks=20
#SBATCH --cpus-per-task=1
#SBATCH --job-name=quantum_job
#SBATCH --output=quantum_job.out
#SBATCH --error=quantum_job.err
#SBATCH --time=10:00:00

module load mpi/openmpi/4.1.2-gcc-10.2.0

mpicxx -o quantum_sim quantum_sim.cpp
mpirun -np 20 ./quantum_sim
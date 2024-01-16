#!/bin/bash
#SBATCH --job-name=matmul
#SBATCH --output=matmul_%j.output
#SBATCH --error=matmul_%j.err
#SBATCH --partition=s_hadoop,s_standard
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --cpus-per-task=72

# Load any necessary modules (if needed)
# module load mymodule


# Enter your executable commands here
# Execute the compiled program
date
g++ matmul.cpp -o matmul.o -march=native -O2
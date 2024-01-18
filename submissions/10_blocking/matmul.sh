#!/bin/bash
#SBATCH --job-name=matmul
#SBATCH --output=matmul_%j.output
#SBATCH --error=matmul_%j.err
#SBATCH --partition=s_hadoop,s_standard
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --cpus-per-task=72
module purge

# Load any necessary modules (if needed)
# module load mymodule
module load compiler/gcc/11.2.0
module load tools/python/3.8
python -m pip install --user matplotlib

# Enter your executable commands here
# Execute the compiled program
date
g++ matmul.cpp -o matmul.o -march=native -O2
./matmul.o
python3 performance.py performance.csv performance.png
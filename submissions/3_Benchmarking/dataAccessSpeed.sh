#!/bin/bash
#SBATCH --job-name=dataAccessSpeed
#SBATCH --output=dataAccessSpeed.output
#SBATCH --error=dataAccessSpeed.err
#SBATCH --partition=s_hadoop,s_standard
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --cpus-per-task=72

# Load any necessary modules (if needed)
# module load mymodule

# Enter your executable commands here
# Execute the compiled program
g++ dataAccessSpeed.cpp -o dataAccessSpeed.out -O3
./dataAccessSpeed.out
python3 visualizeTriad.py
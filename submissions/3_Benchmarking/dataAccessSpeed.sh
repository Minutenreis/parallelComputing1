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
module load tools/python/3.8

# Enter your executable commands here
# Execute the compiled program
date
lscpu
g++ -std=c++1y dataAccessSpeed.cpp -o dataAccessSpeed.out -O
./dataAccessSpeed.out
python3 visualizeTriad.py
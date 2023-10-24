#!/bin/bash
#SBATCH --job-name=fib
#SBATCH --output=fib.out
#SBATCH --error=fib.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:05:00
#SBATCH --cpus-per-task= TODO: how many sinfo -p s_hadoop

# Load any necessary modules (if needed)
# module load mymodule

# Enter your executable commands here
# Execute the compiled program
echo "Hello, SLURM job!" # TODO: replace with your commands

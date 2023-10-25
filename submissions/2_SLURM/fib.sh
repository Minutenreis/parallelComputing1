#!/bin/bash
#SBATCH --job-name=fib
#SBATCH --output=fib.out
#SBATCH --error=fib.err
#SBATCH --partition=s_hadoop,s_standard
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=10:00
#SBATCH --cpus-per-task=72

# Load any necessary modules (if needed)
# module load mymodule

# Enter your executable commands here
# Execute the compiled program
start_time=$(date +"%s")
g++ fib.cpp -o fib.o
./fib.o
end_time=$(date +"%s")
elapsed_time=$((end_time - start_time))
echo "Job completed in $(date -u -d @$elapsed_time +'%H:%M:%S')"

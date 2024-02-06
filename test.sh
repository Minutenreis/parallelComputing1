#!/bin/bash
#SBATCH --job-name=lscpu
#SBATCH --output=lscpu.output
#SBATCH --error=lscpu.err
#SBATCH --partition=s_hadoop,s_standard
#SBATCH --nodes=10
#SBATCH --ntasks=1
#SBATCH --time=1:00
#SBATCH --cpus-per-task=480

lscpu
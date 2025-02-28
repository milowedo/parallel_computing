#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive 
#SBATCH --constraint="intel"
#SBATCH --time=00:08:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmilowedo2020a

module add plgrid/tools/openmpi

mpicc -o carlo carlo_csv.c -lm

for ((i=1; i<=$1; i++)); do
    mpiexec -np $i ./carlo $2 0
done

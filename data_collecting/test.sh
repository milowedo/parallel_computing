#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:15:00
#SBATCH --partition=plgrid-testing
#SBATCH --account=plgmilowedo2020a

module add plgrid/tools/openmpi

mpicc -o carlo carlo_csv.c -lm

     mpiexec -np 12 ./carlo 1000000000

#!/bin/bash

mpicc -o ex2 distribute_times.c
mpicc -o ex3 myScatter.c

echo "2 processes"
echo "mpi implemented scatter"
mpiexec --hostfile allnodes -np 2 ./ex2
sleep 3s

echo "-----------------------------------"
echo "self implemented scatter"
mpiexec --hostfile allnodes -np 2 ./ex3
sleep 3s



echo "-----------------------------------"
echo "8 processes"
echo "mpi implemented scatter"
mpiexec --hostfile allnodes -np 8 ./ex2
sleep 3s

echo "-----------------------------------"
echo "self implemented scatter"
mpiexec --hostfile allnodes -np 8 ./ex3
sleep 3s


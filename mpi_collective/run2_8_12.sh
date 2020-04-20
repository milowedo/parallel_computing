#!/bin/bash

mpicc -o ex2 distribute_times.c

echo "2 processes"
mpiexec --hostfile allnodes -np 2 ./ex2
sleep 3s

echo "-----------------------------------"
echo "8 processes"
mpiexec --hostfile allnodes -np 8 ./ex2
sleep 3s

echo "-----------------------------------"
echo "12 processes"
mpiexec --hostfile allnodes -np 12 ./ex2
sleep 3s
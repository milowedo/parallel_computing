#!/bin/bash 

gcc -fopenmp fill_array_async.c

for ((i=1; i<=10; i++)); do
     ./a.out
done

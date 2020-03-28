#!/bin/bash

for file in slurm-*.out
do cat $file | grep 000000 >> results_combined.csv
done

#!/bin/bash

for file in slurm-*.out
do tail -n +4 $file >> results_combined.csv
done

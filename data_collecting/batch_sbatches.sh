#!/bin/bash

#arg1 repetitions
#arg2 number of points

for ((i=1; i<=$1; i++));
do
    cd noscale
    sbatch wsad_dont_scale.sh 12 $2
    cd ../scale
    sbatch wsad_scale.sh 12 $2
    sleep 60
    cd ..
done

cd noscale
for file in slurm-*.out 
do tail -n +4 $file >> results_combined.csv 
done

cd ../scale
for file in slurm-*.out 
do tail -n +4 $file >> results_combined.csv 
done

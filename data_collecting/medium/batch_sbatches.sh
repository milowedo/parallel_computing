#!/bin/bash

#arg1 repetitions
#arg2 number of points

for ((i=1; i<=$1; i++));
do
    cd noscale
    sbatch wsad_dont_scale.sh 12 $2
    cd ../scale
    sbatch wsad_scale.sh 12 $2
    sleep 3600
    cd ..
done

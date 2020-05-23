#!/bin/bash

mul=multiplication_stats.csv
mul_cublas=cublas_mult_stats.csv
A="A_"
B="B_"

if [ -f $mul ]; then
rm $mul
 fi


task=mul.out

for k in {1..10};
do
let j=$k*64
let max_rep=$j*$j/256
if [ -f $B$j ]; then rm $B$j; fi;

for (( i=1; i<=$max_rep; i++ )); do cat B_16 >> $B$j; done;

if [ -f $A$j ]; then rm $A$j; fi;

for (( i=1; i<=$max_rep; i++ )) do cat A_16 >> $A$j; done;

echo $j
	for i in {1..50}
	do
	./$task $j $A$j $B$j >> $mul
	done;
done

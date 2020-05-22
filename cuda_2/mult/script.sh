#!/bin/bash

mul=multiplication_stats.csv
mul_cublas=cublas_mult_stats.csv
A="A_"
B="B_"

if [ -f $mul ]; then
rm $mul
 fi


task=mul.out

for j in 16 32 160 240 320 400
do
	for i in {1..1}
	do
	./$task $j $A$j $B$j >> $mul
	done
done

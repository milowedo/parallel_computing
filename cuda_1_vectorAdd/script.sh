#!/bin/bash

cpu_size=cpu_size_compare.csv
gpu_size=gpu_size_compare.csv

gpu_block=gpu_block_thread_compare.csv

if [ -f $cpu_size ]; then     rm $cpu_size
fi

if [ -f $gpu_size ]; then     rm $gpu_size
fi

if [ -f $gpu_block ]; then     rm $gpu_block
fi

for size in 10000 100000 1000000
do
let block_size=1024
let num_blocks=(size+block_size-1)/block_size
	for i in {1..50}
	do
	./vector_add_sync.out $size >> $cpu_size
	./vector_add.out $size $num_blocks $block_size >> $gpu_size
	done
done


for i in 1 2 4 8 16
do
let NUM_BLOCK=256*$i
let array_size=1000000
let block_size=512
	for i in {1..50}
	do
        ./vector_add.out $array_size $NUM_BLOCK $block_size >> $gpu_block
	done
done

echo $'\n' >> $gpu_block

for j in 1 2 4 8 16
do
let BLOCK_SIZE=1024/j
let array_size=1000000
let num_blocks=64
	for i in {1..50}
	do
	./vector_add.out $array_size $num_blocks $BLOCK_SIZE >> $gpu_block
	done
done

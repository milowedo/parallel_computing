#!/bin/bash

mpicc -o sync metrics.c 
mpicc -o buff metrics_buffered.c 

# echo "1 node shared memory"
# mpiexec --host vnode-04.dydaktyka.icsr.agh.edu.pl:2 -np 2 ./sync 1node_shmem_sync
# sleep 5s
# mpiexec --host vnode-04.dydaktyka.icsr.agh.edu.pl:2 -np 2 ./buff 1node_shmem_buff
# sleep 5s

# echo "1 node tcp"
# mpiexec --host vnode-04.dydaktyka.icsr.agh.edu.pl:2 -np 2 -env MPICH_NO_LOCAL 1  -env MPIR_CVAR_CH3_NOLOCAL 1 ./sync 1node_tcp_sync
# sleep 5s
# mpiexec --host vnode-04.dydaktyka.icsr.agh.edu.pl:2 -np 2 -env MPICH_NO_LOCAL 12 -env MPIR_CVAR_CH3_NOLOCAL 1 ./buff 1node_tcp_buff
# sleep 5s

echo "2 nodes(vm) on 1 physical machine"
mpiexec --hostfile nodes1_3 -np 2 ./sync 2nodes_1_3_sync
sleep 5s
mpiexec --hostfile nodes1_3 -np 2 ./buff 2nodes_1_3_buff
sleep 5s

echo "2 nodes(vm) each on separate physical machine"
mpiexec --hostfile nodes5_12 -np 2 ./sync 2nodes_5_12_buff
sleep 5s
mpiexec --hostfile nodes5_12 -np 2 ./buff 2nodes_5_12_sync
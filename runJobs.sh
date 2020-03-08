#!/bin/bash

mpicc -o sync metrics.c 
mpicc -o buff metrics_buffered.c 

mpiexec --host vnode-04.dydaktyka.icsr.agh.edu.pl:2 -np 2 ./sync 1node_shmem_sync
mpiexec --host vnode-04.dydaktyka.icsr.agh.edu.pl:2 -np 2 ./buff 1node_shmem_buff


mpiexec --host vnode-04.dydaktyka.icsr.agh.edu.pl:2 -np 2 -env MPICH_NO_LOCAL 1  -env MPIR_CVAR_CH3_NOLOCAL 1 ./sync 1node_tcp_sync
mpiexec --host vnode-04.dydaktyka.icsr.agh.edu.pl:2 -np 2 -env MPICH_NO_LOCAL 12 -env MPIR_CVAR_CH3_NOLOCAL 1 ./buff 1node_tcp_buff


mpiexec --hostfile nodes1_3 -np 2 ./sync 2nodes_1_3_sync
mpiexec --hostfile nodes1_3 -np 2 ./buff 2nodes_1_3_buff


mpiexec --hostfile nodes5_12 -np 2 ./sync 2nodes_1_3_buff
mpiexec --hostfile nodes5_12 -np 2 ./obuffut 2nodes_1_3_sync
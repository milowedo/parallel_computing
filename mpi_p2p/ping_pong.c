#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
    int world_rank, world_size;
    int max_amount_of_hits = 20;
    if (argc > 2)
    {
        max_amount_of_hits = atoi(argv[1]);
    }

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int hit = 0;
    MPI_Request request;
    MPI_Status status;

    //initial hit
    if (world_rank == 0)
    {
        hit++;
        MPI_Isend(&hit, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);
        printf("%d PING (%d) \n", world_rank, hit);
        goto FIRST;
    }

    LOOP_START:
    if (hit > max_amount_of_hits)
        goto END;
    if (world_rank == 0)
    {
        MPI_Irecv(&hit, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        hit++;
        MPI_Isend(&hit, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);
        printf("%d PING (%d) \n", world_rank, hit);
        fflush(stdout);
    }
    else if (world_rank == 1)
    {
        MPI_Irecv(&hit, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        hit++;
        MPI_Isend(&hit, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
        printf("%d PONG (%d) \n", world_rank, hit);
        fflush(stdout);
    }

FIRST:
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    goto LOOP_START;

END:
    MPI_Finalize();
    return 0;
}
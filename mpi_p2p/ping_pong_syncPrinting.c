#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int world_rank, world_size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size < 3)
        goto END;

    int max_amount_of_hits = 12;
    int hit = 0;
    MPI_Request request;
    MPI_Status status;
    char str[100];
    char result[100];

    //initial hit
    if (world_rank == 1)
    {
        hit++;
        MPI_Isend(&hit, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &request);

        sprintf(str, "%d PING (%d) \n", world_rank, hit);
        MPI_Send(str, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

        fflush(stdout);
        goto FIRST;
    }

LOOP_START:

    if(world_rank == 0){
        if(hit >= max_amount_of_hits) goto END;
    }else if(world_rank == 1){
        if(hit >= max_amount_of_hits -1) goto END;
    }else if(world_rank == 2){
        if(hit >= max_amount_of_hits) goto END;
    }

    if (world_rank == 0) //PRINTER
    {
        printf("%d loop start hit is: %d\n", world_rank, hit);
        MPI_Recv(&result, 100, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        hit++;
        printf("%s", result);
        MPI_Recv(&result, 100, MPI_CHAR, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        hit++;
        printf("%s", result);
    }
    if (world_rank == 1) //PINGER
    {
        MPI_Irecv(&hit, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        hit++;
        MPI_Isend(&hit, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &request);

        sprintf(str, "%d PING (%d) \n", world_rank, hit);
        MPI_Send(str, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 2) //PONGER
    {

        MPI_Irecv(&hit, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        hit++;
        MPI_Isend(&hit, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);

        sprintf(str, "%d PONG (%d) \n", world_rank, hit);
        MPI_Send(str, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

FIRST:
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    goto LOOP_START;

END:
    MPI_Finalize();
    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fill_with_as(int num_elements)
{
    char *as_array = (char *)malloc(sizeof(char) * num_elements);
    int i;
    for (i = 0; i < num_elements; i++)
    {
        as_array[i] = 'a';
    }
    return as_array;
}

int main(int argc, char **argv)
{
    int world_rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //print_processor_info(world_rank, world_size);

    char *as_array = NULL;
    int arraySize = 100 * 1000000;
    int per_each_process = arraySize / world_size;
    double global_start_scatter_time, global_start_gather_time;
    if (world_rank == 0)
    {
        as_array = fill_with_as(arraySize);
    }

    // Create a buffer that will hold a subset
    char *subset_of_as = (char *)malloc(sizeof(char) * per_each_process);

    MPI_Barrier(MPI_COMM_WORLD);
    global_start_scatter_time = MPI_Wtime();

    // MPI_Scatter(as_array, per_each_process, MPI_CHAR,
    //             subset_of_as, per_each_process, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        // If we are the root process, send our data to everyone
        int i;
        for (i = 0; i < world_size; i++)
        {
            if (i != world_rank)
            {
                int j;
                for (j = 0; j < per_each_process; j++)
                {
                    subset_of_as[j] = as_array[((world_rank)*per_each_process)+j];
                }
                MPI_Send(subset_of_as, per_each_process, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            }

        }
    }
    else
    {
        // If we are a receiver process, receive the data from the root
        MPI_Recv(subset_of_as, per_each_process, MPI_CHAR, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    printf("Scatter ended for %d after %lf\n", world_rank, MPI_Wtime() - global_start_scatter_time);

    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0)
    {
        printf("Scatter ended global after %lf\n", MPI_Wtime() - global_start_scatter_time);
    }

    char *results = NULL;
    if (world_rank == 0)
    {
        results = (char *)malloc(sizeof(char) * arraySize);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    global_start_gather_time = MPI_Wtime();

    // int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    //                void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
    MPI_Gather(subset_of_as, per_each_process, MPI_CHAR,
               results, per_each_process, MPI_CHAR, 0, MPI_COMM_WORLD);

    printf("Gather ended for %d after %lf\n", world_rank, MPI_Wtime() - global_start_gather_time);

    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0)
    {
        printf("Gather ended global after %lf\n", MPI_Wtime() - global_start_gather_time);
    }

    //cleanup
    if (world_rank == 0)
    {
        free(results);
        free(as_array);
    }
    free(subset_of_as);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}

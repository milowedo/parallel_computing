#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_processor_info(int world_rank, int world_size)
{
    int len;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(name, &len);
    printf("I am process %d of total %d on cpu %s\n", world_rank, world_size, name);
    fflush(stdout);
}

int *fill_with_ones(int num_elements)
{
    int *numbers_array = (int *)malloc(sizeof(int) * num_elements);
    int i;
    for (i = 0; i < num_elements; i++)
    {
        numbers_array[i] = 1;
    }
    return numbers_array;
}

int main(int argc, char **argv)
{
    int world_rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    print_processor_info(world_rank, world_size);

    int *ones_array = NULL;
    int numbers_per_rank = 3;
    if (world_rank == 0)
    {
        ones_array = fill_with_ones(world_size * numbers_per_rank);
    }
    
    // Create a buffer that will hold a subset of the random numbers
    int *subset_of_numbers = (int *)malloc(sizeof(int) * numbers_per_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    // int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    //                 void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
    // MPI_Scatter(ones_array, numbers_per_rank, MPI_INT,
    //             subset_of_numbers, numbers_per_rank, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        // If we are the root process, send our data to everyone
        int i;
        for (i = 0; i < world_size; i++)
        {
            if (i != world_rank)
            {
                int j;
                for (j = 0; j < numbers_per_rank; j++)
                {
                    subset_of_numbers[j] = ones_array[((world_rank)*numbers_per_rank)+j];
                }
                MPI_Send(subset_of_numbers, numbers_per_rank, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    }
    else
    {
        // If we are a receiver process, receive the data from the root
        MPI_Recv(subset_of_numbers, numbers_per_rank, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    //multiple by world_rank
    int i;
    for (i = 0; i < numbers_per_rank; i++)
    {
        subset_of_numbers[i] *= world_rank;
    }

    int *results = NULL;
    if (world_rank == 0)
    {
        results = (int *)malloc(sizeof(int) * world_size * numbers_per_rank);
    }
    // int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    //                void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);
    MPI_Gather(subset_of_numbers, numbers_per_rank, MPI_INT,
               results, numbers_per_rank, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        int i;
        for (i = 0; i < numbers_per_rank * world_size; i++)
        {
            printf("%d ", results[i]);
        }
        printf("\n");
    }

    //cleanup
    if (world_rank == 0)
    {
        free(results);
        free(ones_array);
    }
    free(subset_of_numbers);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    //printf("FINISHED at %d\n", world_rank);
    return 0;
}

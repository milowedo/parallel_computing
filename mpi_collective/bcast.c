#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./out num_elements num_trials\n");
        exit(1);
    }
    int num_elements = atoi(argv[1]);
    int num_trials = atoi(argv[2]);

    int world_rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int len;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(name, &len);
    printf("I am process %d of total %d on cpu %s\n", world_rank, world_size, name);
    fflush(stdout);

    int *data = (int *)malloc(sizeof(int) * num_elements);
    // int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
    MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    printf("All processes received information, I am: %d\n", world_rank);

    // Print off timing information
    if (world_rank == 0)
    {
        printf("Data size = %d, Trials = %d\n", num_elements * (int)sizeof(int),
               num_trials);
    }

    MPI_Finalize();
    printf("FINISHED at %d\n", world_rank);
    return 0;
}

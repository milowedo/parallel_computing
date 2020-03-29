#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

int main(int argc, char **argv)
{

    //Arguments are :
    //arg1 : int - denotes the number of points one want to have generated
    //arg2 : int - if arg1 number of points are to be generated in summary(default - value=1) or on each node (value=0)

    ///
    int world_rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int len;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(name, &len);

    unsigned long long num_of_points = 100 * 1000000, points_in_circle_global = 0, local_points_num, local_circle_points = 0, i;
    float x, y, dist, PI;
    char *endPtr;
    double global_start = 0, global_stop = 0, global_time;
    // double start = 0, stop = 0, ttime = 0;

    if (argc < 2)
    {
        printf("You have not passed the number of points as argument - assuming %llu\n", num_of_points);
        fflush(stdout);
    }
    else
    {
        num_of_points = strtoull(argv[1], &endPtr, 10);
    }

    if (argc > 2 && atoi(argv[2]) == 0)
    {
        local_points_num = num_of_points;
    }
    else
    {
        local_points_num = num_of_points / world_size;
    }
    
    //
    MPI_Barrier(MPI_COMM_WORLD);

    //
    if (world_rank == 0)
    {
	printf("%llu,%d", num_of_points, world_size);
        global_start = MPI_Wtime();
    }

    //
    srand(time(NULL));
    for (i = 0; i < local_points_num; i++)
    {
        x = rand() / (float)RAND_MAX;
        y = rand() / (float)RAND_MAX;
        dist = sqrt(pow(x, (float)2.0) + pow(y, (float)2.0));
        if (dist <= 1)
        {
            local_circle_points++;
        }
    }

    //
    MPI_Reduce(&local_circle_points, &points_in_circle_global, 1, MPI_INT, MPI_SUM, 0,
               MPI_COMM_WORLD);

    //
    if (world_rank == 0)
    {
        global_stop = MPI_Wtime();
        global_time = global_stop - global_start;
    }

    //
    if (world_rank == 0)
    {
        PI = (float)(4.0) * ((float)(points_in_circle_global) / (float)(num_of_points));
        if(num_of_points == local_points_num){
            PI = PI / (float) world_size;
        }
        printf(",%lf\n", global_time);
        
        fflush(stdout);
    }

    //
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}

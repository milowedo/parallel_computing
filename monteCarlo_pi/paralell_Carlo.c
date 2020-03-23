#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main(int argc, char **argv)
{

    ///
    int world_rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int len;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(name, &len);

    unsigned long long num_of_points = 100 * 1000000, points_in_circle_global = 0, local_points_num, local_circle_points = 0;
    int i;
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
        // if (world_rank == 0)
        // {
        //     printf("Running for total of %llu points\n", num_of_points);
        //     fflush(stdout);
        // }
    }
    local_points_num = num_of_points / world_size;
    printf("Running for %llu points on %s\n", local_points_num, name);
    fflush(stdout);

    //
    MPI_Barrier(MPI_COMM_WORLD);
    
    //
    if (world_rank == 0)
    {
        global_start = MPI_Wtime();
    }

    //
    srand(time(NULL));
    //start = MPI_Wtime();
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

    // stop = MPI_Wtime();
    // ttime = stop - start;
    // printf("%lf on %s\n", ttime, name);
    // fflush(stdout);

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
        printf("PI: %lf\n", PI);
        printf("Total time: %lf\n", global_time);
        fflush(stdout);
    }

    
    //
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}

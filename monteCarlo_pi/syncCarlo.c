#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main(int argc, char **argv)
{
    int world_rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    unsigned long long num_of_points = 100 * 1000000, points_in_circle = 0;
    int i;
    float x, y, dist, PI;
    char* endPtr;

    if (argc < 2)
    {
        printf("You have not passed the number of points as argument - assuming %llu\n", num_of_points);
    }
    else
    {
        num_of_points = strtoull(argv[1], &endPtr, 10);
        printf("Running for %llu points\n", num_of_points);
    }
    srand(time(NULL));
    for (i = 0; i < num_of_points; i++)
    {
        x = rand() / (float)RAND_MAX;
        y = rand() / (float)RAND_MAX;
        dist = sqrt(pow(x, (float)2.0) + pow(y, (float)2.0));
        if (dist <= 1)
        {
            points_in_circle++;
        }
    }

    PI = (float)(4.0) * ((float)(points_in_circle) / (float)(num_of_points));

    printf("%lf\n", PI);

    MPI_Finalize();
    return 0;
}

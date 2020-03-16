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

    int num_of_points = 100 * 1000, i, points_in_circle = 0;
    float x, y, dist, PI;

    if (argc < 2)
    {
        printf("You have not passed the number of points as argument - assuming %d\n", num_of_points);
    }
    else
    {
        num_of_points = atoi(argv[1]);
        printf("Running for %d points\n", num_of_points);
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

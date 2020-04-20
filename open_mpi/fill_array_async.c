#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int i, num_iterations, chunk_size;

    num_iterations = 200 * 1000000;
    int *integer_array = (int *)malloc(sizeof(int) * num_iterations);
    double begin_t, end_t;

#pragma omp parallel default(none) private(i) shared(num_iterations, chunk_size, integer_array, begin_t, end_t)
    {

#pragma omp single
        {
            chunk_size = num_iterations / omp_get_num_threads() /4/4/4;
        }

        unsigned int myseed = omp_get_thread_num();

        begin_t = omp_get_wtime();
#pragma omp for schedule(runtime)
        for (i = 0; i < num_iterations; i++)
        {
            integer_array[i] = rand_r(&myseed);
        }
        end_t = omp_get_wtime();
    }

    printf("%f\n", ((double)(end_t - begin_t)));

    return 0;
}
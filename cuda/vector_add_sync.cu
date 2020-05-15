#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <helper_timer.h>

void add(int* a, int* b, int* c, int N)
{ 
    int i;
    for (i = 0; i < N; i ++)
    { 
        c[i] = a[i] + b[i];
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "You have to provide ARRAY_SIZE as argument.\n");
        return -1;
    }

    char *p;

    int ARRAY_SIZE;
    int ul_conv = strtoul(argv[1], &p, 10);
    ARRAY_SIZE = ul_conv;

    int N = ARRAY_SIZE;
    int* a = (int *)malloc(N * sizeof(int));
    int* b = (int *)malloc(N * sizeof(int));
    int* c = (int *)malloc(N * sizeof(int));

    int i;
    for (i = 0; i < N; i++)
    {
        a[i] = i;
        b[i] = i * 2;
    }

   
    StopWatchInterface *timer=NULL;
    sdkCreateTimer(&timer);
    sdkResetTimer(&timer);
    sdkStartTimer(&timer);

    add(a, b, c, N);

    sdkStopTimer(&timer);
    float time = sdkGetTimerValue(&timer);
    sdkDeleteTimer(&timer);


    //for (int i = 0; i < N; i++)
    //{
    //    printf("%d + %d = %d\n", a[i], b[i], c[i]);
 
    //}

    // cleanup
    free(a);
    free(b);
    free(c);
     
    printf ("%d, %f\n", ARRAY_SIZE, time);
    return 0;
}

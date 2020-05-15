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

int main(void)
{
    if (argc != 4)
    {
        fprintf(stderr, "You have to provide ARRAY_SIZE, THREAD_COUNT, BLOCK_COUNT and  as arguments.\n");
        return -1;
    }
    
    char *p;

    int ARRAY_SIZE;
    ul_conv = strtoul(argv[1], &p, 10);
    ARRAY_SIZE = ul_conv;

    int THREAD_COUNT;
    ul_conv = strtoul(argv[2], &p, 10);
    THREAD_COUNT = ul_conv;

    int BLOCK_COUNT;
    ul_conv = strtoul(argv[3], &p, 10);
    BLOCK_COUNT = ul_conv;

    int N = 10;
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
     
    printf ("Time for the kernel: %f ms\n", time);
    return 0;
}

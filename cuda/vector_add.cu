#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <helper_timer.h>

__global__ void add(int *a, int *b, int *c, int N)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    for (int i = index; i < N; i += stride)
    { 
        c[i] = a[i] + b[i];
    }
}

int main(int argc, char* argv[])
{

    if (argc != 4)
    {
        fprintf(stderr, "You have to provide ARRAY_SIZE, NUM_BLOCK, BLOCK_SIZE and  as arguments.\n");
        return -1;
    }
    
    char *p;

    int ARRAY_SIZE;
    int ul_conv = strtoul(argv[1], &p, 10);
    ARRAY_SIZE = ul_conv;

    int NUM_BLOCK;
    ul_conv = strtoul(argv[2], &p, 10);
    NUM_BLOCK = ul_conv;

    int BLOCK_SIZE;
    ul_conv = strtoul(argv[3], &p, 10);
    BLOCK_SIZE = ul_conv;

    int N = ARRAY_SIZE;
    int a[N], b[N], c[N];

    int *dev_a, *dev_b, *dev_c;
    cudaMalloc((void **)&dev_a, N * sizeof(int));
    cudaMalloc((void **)&dev_b, N * sizeof(int));
    cudaMalloc((void **)&dev_c, N * sizeof(int));

    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        b[i] = i * 2;
    }

    cudaMemcpy(dev_a, a, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_c, c, N * sizeof(int), cudaMemcpyHostToDevice);
   
    StopWatchInterface *timer=NULL;
    sdkCreateTimer(&timer);
    sdkResetTimer(&timer);
    sdkStartTimer(&timer);

    add<<<NUM_BLOCK, BLOCK_SIZE>>>(dev_a, dev_b, dev_c, N);

    cudaDeviceSynchronize();
    cudaThreadSynchronize();
    sdkStopTimer(&timer);
    float time = sdkGetTimerValue(&timer);
    sdkDeleteTimer(&timer);

    cudaMemcpy(c, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost);

    //for (int i = 0; i < N; i++)
    //{
    //    printf("%d + %d = %d\n", a[i], b[i], c[i]);
    // }

    // cleanup
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    printf ("%d, %d, %d, %f\n", ARRAY_SIZE, NUM_BLOCK, BLOCK_SIZE, time);
    return 0;
}

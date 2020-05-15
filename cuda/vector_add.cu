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

int main(void)
{
    int N = 10;
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

    add<<<1, 1>>>(dev_a, dev_b, dev_c, N);

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

    printf ("Time for the kernel: %f ms\n", time);
    return 0;
}

// Matrix multiplication by parts
// Elements stored in row-major order

using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <helper_timer.h>
#define BLOCK_SIZE 16

typedef struct
{	int WIDTH;
	int height;
	float *elements;
} Matrix;

// Forward declaration of matrix mult
__global__ void MatMulKernel (const Matrix, const Matrix, Matrix);

// Host code
void MatMul(const Matrix A, const Matrix B, Matrix C)
{
	// Load matrices A and B to device memory
	Matrix d_A;
	d_A.WIDTH = A.WIDTH; d_A.height = A.height;
	size_t size = A.WIDTH * A.height * sizeof(float);
	cudaMalloc((void**) &d_A.elements, size);
	cudaMemcpy(d_A.elements, A.elements, size, cudaMemcpyHostToDevice);
	
	Matrix d_B;
	d_B.WIDTH = B.WIDTH; d_B.height = B.height;
	size = B.WIDTH * B.height * sizeof(float);
	cudaMalloc((void**) &d_B.elements, size);
	cudaMemcpy(d_B.elements, B.elements, size, cudaMemcpyHostToDevice);
	
	// allocate C in device
	Matrix d_C;
	d_C.WIDTH = C.WIDTH; d_C.height = C.height;
	size = d_C.WIDTH * d_C.height * sizeof(float);
	cudaMalloc((void**) &d_C.elements, size);
	
	// Invoke kernel
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid(B.WIDTH / dimBlock.x, A.height / dimBlock.y);
    MatMulKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C);
	
	// copy C to host
	cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost);
	
	// free device memory
	cudaFree(d_A.elements);
	cudaFree(d_B.elements);
	cudaFree(d_C.elements);
}

//matrix multiplication kernel
__global__ void MatMulKernel(Matrix A, Matrix B, Matrix C)
{
	// each thread computes one element of C and acumulates results to Cvalue
	 float Cvalue = 0;
	 int row = blockIdx.y * blockDim.y + threadIdx.y;
	 int col = blockIdx.x * blockDim.x + threadIdx.x;
	if ((row>=A.height) || (col>=B.WIDTH))
	{
		return;
	}
	for (int e=0; e<A.WIDTH; e++){
		Cvalue += A.elements[row*A.WIDTH + e] * B.elements[e*B.WIDTH + col];
		C.elements[row*C.WIDTH + col] = Cvalue;
	}
}

//square matrix multiplication cpu
void MatMulCPU(Matrix A, Matrix B, Matrix C)
{
	int i,j,k, n = A.WIDTH;
	for(i = 0; i < n; ++i){
		for(j = 0; j < n; ++j)
		{
			C.elements[i*C.WIDTH +j] = 0;
			for(k = 0; k < n; ++k)
            {
                C.elements[i*C.WIDTH +j] += A.elements[i*A.WIDTH +k] * B.elements[k*A.WIDTH +j];
            }
		} 
	}
		        
}

int main(int argc, char * const argv[])
{	
	if (argc != 4)
    {
        fprintf(stderr, "You have to provide WIDTH, ARRAY_A, ARRAY_B  as arguments.\n");
        return -1;
    }
    
    char *p;

    int WIDTH;
    WIDTH = strtoul(argv[1], &p, 10);

    char* ARRAY_A;
    ARRAY_A = argv[2];

    char* ARRAY_B;
	ARRAY_B = argv[3];
	
	StopWatchInterface *timer=NULL;
    sdkCreateTimer(&timer);
		
	Matrix A;
	Matrix B;
	Matrix C_gpu;
	Matrix C_cpu;
	
	A.WIDTH = WIDTH;
	B.WIDTH = WIDTH;
	C_gpu.WIDTH = WIDTH;
	C_cpu.WIDTH = WIDTH;
	
	A.height = WIDTH;
	B.height = WIDTH;
	C_gpu.height = WIDTH;
	C_cpu.height = WIDTH;
	
	A.elements = new float[WIDTH*WIDTH];
	B.elements = new float[WIDTH*WIDTH];
	C_gpu.elements = new float[WIDTH*WIDTH];
	C_cpu.elements = new float[WIDTH*WIDTH];
	
	//fill matrices
	std::ifstream A_input;
	std::ifstream B_input;
	A_input.open(ARRAY_A);
	B_input.open(ARRAY_B);
	
	float a, b;
	A_input >> a;	
	B_input >> b;	
	int i = 0;
	while (!A_input.eof())
	{	A.elements[i] = a;
		B.elements[i] = b;
		A_input >> a;	
		B_input >> b;	
		i += 1;
	}
	A_input.close();
	B_input.close();

	sdkResetTimer(&timer);
        sdkStartTimer(&timer);
	MatMul(A, B, C_gpu);
        sdkStopTimer(&timer);
	float time_gpu = sdkGetTimerValue(&timer);
	
	sdkResetTimer(&timer);
	sdkStartTimer(&timer);
	MatMulCPU(A, B, C_cpu);
	float time_cpu = sdkGetTimerValue(&timer);

	//check if the same
	for (int i=0; i<WIDTH; i++)
	{	for (int j=0; j<WIDTH; j++)
			if (C_gpu.elements[i*WIDTH+j] != C_cpu.elements[i*WIDTH+j]){
				printf("something is no yes");
			}
	}

	std::ofstream C_output;
	C_output.open("C.txt");
	for (int i=0; i<WIDTH; i++)
	{	for (int j=0; j<WIDTH; j++)
			C_output<<C_cpu.elements[i*WIDTH+j]<<"\t";
		C_output<<endl;
	}

	printf ("%d, %f, %f\n", WIDTH, time_gpu, time_cpu);
	sdkDeleteTimer(&timer);
}	

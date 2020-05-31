/*** Calculating a derivative with CD ***/
#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/time.h>

void copy_array(float *u, float *u_prev, int N)
{
    int i;
    for(i = 0; i< N*N; i++){
        u_prev[i] = u[i];
    }
}

void update (float *u, float *u_prev, int N, float h, float dt, float alpha)
{
	// Setting up indices
	int I = 0;
    
    for(; I < N*N; I++){
        if ( (I>N) && (I< N*N-1-N) && (I%N!=0) && (I%N!=N-1)) 
        {	
            u[I] = u_prev[I] + alpha*dt/(h*h) * (u_prev[I+1] + u_prev[I-1] + u_prev[I+N] + u_prev[I-N] - 4*u_prev[I]);
        }
    }
	
	// Boundary conditions are automatically imposed
	// as we don't touch boundaries
}

int main()
{
    int N = 128;

	float xmin 	= 0.0f;
	float xmax 	= 3.5f;
	float ymin 	= 0.0f;
	//float ymax 	= 2.0f;
	float h   	= (xmax-xmin)/(N-1);
	float dt	= 0.00001f;	
	float alpha	= 0.645f;
	float time 	= 0.4f;

	int steps = ceil(time/dt);
	int I;

	float *x  	= new float[N*N]; 
	float *y  	= new float[N*N]; 
	float *u  	= new float[N*N];
	float *u_prev  	= new float[N*N];


	// Generate mesh and intial condition
	for (int j=0; j<N; j++)
	{	for (int i=0; i<N; i++)
		{	I = N*j + i;
			x[I] = xmin + h*i;
			y[I] = ymin + h*j;
			u[I] = 0.0f;
			if ( (i==0) || (j==0)) 
				{u[I] = 200.0f;}
		}
	}

	// Loop 
	double start = get_time();
	for (int t=0; t<steps; t++)
	{	copy_array(u_d, u_prev_d, N);
		update(u_d, u_prev_d, N, h, dt, alpha);

	}
	double stop = get_time();
	
	double elapsed = stop - start;
	std::cout<<""<<elapsed<<std::endl;

	std::ofstream temperature("temperature_cpu.txt");
	for (int j=0; j<N; j++)
	{	for (int i=0; i<N; i++)
		{	I = N*j + i;
		//	std::cout<<u[I]<<"\t";
			temperature<<x[I]<<"\t"<<y[I]<<"\t"<<u[I]<<std::endl;
		}
		temperature<<"\n";
		//std::cout<<std::endl;
	}

	temperature.close();
}

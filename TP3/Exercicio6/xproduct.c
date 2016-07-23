#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
	#define N 2
#endif

float* generateRandomArray() {
    srand(time(NULL));
    float *array[N];		
	for(int i=0;i<N;i++){		
		array[i] = rand();							
	}	    
    return array;
}

int main (int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, nproc,localsum=0,globalsum=0;	
	MPI_Request request;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	float recvbufA[N/nproc];
	float recvbufB[N/nproc];

	float A[N] = generateRandomArray();
	float B[N] = generateRandomArray();
	MPI_Scatter(A,N/nproc,MPI_FLOAT,recvbufA,N/nproc,MPI_FLOAT,0,MPI_COMM_WORLD);
	MPI_Scatter(B,N/nproc,MPI_FLOAT,recvbufB,N/nproc,MPI_FLOAT,0,MPI_COMM_WORLD);	
	
	for(int i=0;i<N/nproc;i++){
		//localsum =  localsum + recvbufA[i]*recvbufB[i];
		printf("%f\n", recvbufA[i]);
		//printf("%f\n", recvbufB[i]);
	}		
	
	MPI_Reduce(&localsum,&globalsum,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);

	if(rank==0){
		//printf("%d\n", globalsum);
	}	
		
	MPI_Finalize();
	return 0;
}
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
	#define N 10000
#endif


int main (int argc, char** argv)
{	
	struct timeval stop, start;
  	float elapsed;  
  	gettimeofday(&start,NULL);

	MPI_Init(&argc, &argv);
	int rank, nproc,right,left;	
	MPI_Request request;
	MPI_Status status;
	unsigned long count = (unsigned long) N * (unsigned long)N;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	if(rank==nproc-1){
		right= 0;
		left=rank-1;
	}
	else if(rank==0){
		right= rank+1;
		left=nproc-1;
	}
	else{
		right= rank+1;
		left=rank-1;	
	}

	float **A;
	float **B;
	A = malloc(N*sizeof(float *));
	B = malloc(N*sizeof(float *));	
	int i, j;
	for(i=0;i<N;i++){		
		A[i] = malloc(N*sizeof(float));
		B[i] = malloc(N*sizeof(float));
		for(j=0;j<N;j++){
			A[i][j] = (float) rank;					
		}
	}	

	MPI_Isend(&(A[0][0]),count,MPI_FLOAT,right,0,MPI_COMM_WORLD,&request);
	MPI_Irecv(&(B[0][0]),count,MPI_FLOAT,left,0,MPI_COMM_WORLD,&request);


	MPI_Wait(&request,&status);
	printf("A matriz B do processo: %d\n", rank);
	printf("%f\n", B[0][0]);
	
	free(A[0]);
	free(A);
	free(B[0]);
	free(B);
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==0){
		gettimeofday(&stop,NULL);
    	elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec)/1000.0f;
    	printf("Tempo de execução: %f\n", elapsed/1000.0);      	
	}
	
	MPI_Finalize();
	return 0;
}
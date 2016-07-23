#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef N
	#define N 2
#endif


int main (argc, argv)
int argc;
char **argv;
{
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

	float **A = malloc(sizeof(float)*N);
	float **B = malloc(sizeof(float)*N);
	for(int i=0;i<N;i++){
		A[i] = malloc(sizeof(float)*N);
		B[i] = malloc(sizeof(float)*N);
		for(int j=0;j<N;j++){
			A[i][j] = (float) rank;
		}
	}	

	MPI_Isend(&A,count,MPI_FLOAT,right,0,MPI_COMM_WORLD,&request);
	MPI_Irecv(&B,count,MPI_FLOAT,left,0,MPI_COMM_WORLD,&request);

	MPI_Wait(&request,&status);
	printf("A matriz B do processo: %d\n", rank);
	printf("%f\n", &B[0][0]);

	for (int i = 0; i< N; i++)
	{		
		free(A[i]);
		free(B[i]);
	}
	free(A);
	free(B);

	MPI_Finalize();
	return 0;
}
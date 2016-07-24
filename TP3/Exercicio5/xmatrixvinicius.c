#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
	#define N 2
#endif

float** generateRandomMatrix() {
    srand(time(NULL));
    float **matrix;	
	matrix = malloc(N*sizeof(float *));
	for(int i=0;i<N;i++){		
		matrix[i] = malloc(N*sizeof(float));
		for(int j=0;j<N;j++){
			matrix[i][j] = (float) 1.0;					
		}
	}	    
    return matrix;
}

int main (int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, nproc;	
	MPI_Request request;
	MPI_Status status;
	MPI_Datatype coltype;
	unsigned long count = (unsigned long) N * (unsigned long)N;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	float recvbufA[N];
	float recvbufB[N];
	float **product= NULL;

	if(rank==0){		
		float **A = generateRandomMatrix();
		float **B = generateRandomMatrix();
		product = malloc(sizeof(float)*count);

		MPI_Type_vector(N,1,1,MPI_FLOAT,&coltype);
		MPI_Type_commit(&coltype);

		MPI_Scatter(&(A[0][0]),N,MPI_FLOAT,recvbufA,N,MPI_FLOAT,rank,MPI_COMM_WORLD);
		MPI_Scatter(&(B[0][0]),N,MPI_FLOAT,recvbufB,N,coltype,rank,MPI_COMM_WORLD);	
	}	
	
	float *C = malloc(N*sizeof(float));

	for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				printf("%f\n", recvbufA[i]);
			}
		}
	for(int i=0;i<N;i++){
		C[i] = recvbufA[i]*recvbufB[i];
	}
	
	MPI_Gather(C,N,MPI_FLOAT,product,N,MPI_FLOAT,0,MPI_COMM_WORLD);

	if(rank==0){
		for(int i=0;i<N;i++){
			//for(int j=0;j<N;j++){
				//printf("%f\n", C[i]);
			//}
		}
	}
		
	/*
	MPI_Isend(&(A[0][0]),count,MPI_FLOAT,right,0,MPI_COMM_WORLD,&request);
	MPI_Irecv(&(B[0][0]),count,MPI_FLOAT,left,0,MPI_COMM_WORLD,&request);


	MPI_Wait(&request,&status);
	printf("A matriz B do processo: %d\n", rank);
	printf("%f\n", B[0][0]);

	free(A[0]);
	free(A);
	free(B[0]);
	free(B);
	*/
	MPI_Finalize();
	return 0;
}
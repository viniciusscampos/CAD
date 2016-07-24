#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
	#define N 2
#endif

float** generateRandomMatrix() {
    float **matrix;	
    float a = 5.0; //número máximo para geração
	matrix = malloc(N*sizeof(float *));
	int i, j;
	for(i=0;i<N;i++){		
		matrix[i] = malloc(N*sizeof(float));
		for(j=0;j<N;j++){
			matrix[i][j] = ((float)rand()/(float)(RAND_MAX)) * a;
		}
	}	    
    return matrix;
}

float **allocate_array(int rows, int cols) 
{
  float **result;
  result=(float **)malloc(rows*sizeof(float *));
  result[0]=(float *)malloc(rows*cols*sizeof(float));
  return result;
}

void deallocate_array(float **array, int rows) 
{
  int i;
  for(i=1; i<rows; i++)
	array[i]=NULL;
  free(array[0]);
  free(array);
}

int main (int argc, char** argv)
{
	//srand(time(NULL));
	int rank, nproc, i, j, k;	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Request request;
	MPI_Status status;
	MPI_Datatype rowtype, coltype;
	unsigned long count = (unsigned long) N * (unsigned long)N;

	float **A, **B;
	A = allocate_array(N, N);
	B = allocate_array(N, N);
	float *C = malloc(N*sizeof(float));
	float recvbufA[N];
	float recvbufB[N];
	float recvbufC[N];
		float **product= NULL;

float aux;
	//processo mestre cria as matrizes
	if(rank==0){	
		A=generateRandomMatrix();
		B=generateRandomMatrix();
		for (i=0; i<N; i++){
			for (j=0; j<N; j++){
				printf("A[%d][%d] %f\n",i, j, A[i][j]);
				printf("B[%d][%d] %f\n",i, j, B[i][j]);
			}
		}
		for (i = 0; i < N; i++) {
    for (j = i+1; j < N; j++) {
      if (j != i) {
		   aux = B[i][j];
		   B[i][j] = B[j][i];
		   B[j][i] = aux;
      }
    }
  }	
  	printf("matriz transposta\n");
		for (i=0; i<N; i++){
			for (j=0; j<N; j++){
				printf("B[%d][%d] %f\n",i, j, B[i][j]);
			}
		}	
	}
	//processo mestre particiona as matrizes
	// MPI_Scatter(&A,N*(N/nproc),MPI_FLOAT,recvbufA,N*(N/nproc),MPI_FLOAT,0, MPI_COMM_WORLD);
	// MPI_Bcast(B, N*N, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&(A[0][0]),N,MPI_FLOAT,recvbufA,N,MPI_FLOAT,0,MPI_COMM_WORLD);
	MPI_Scatter(&(B[0][0]),N,MPI_FLOAT,recvbufB,N,MPI_FLOAT,0,MPI_COMM_WORLD);

	for(i=0;i<N;i++){
		C[i] += recvbufA[i]*recvbufB[i];
		printf("receiveA[%d] no rank %d, %f\n",i, rank, recvbufA[i]);
	}
	
	MPI_Gather(&C,N,MPI_FLOAT,recvbufC,N,MPI_FLOAT,0,MPI_COMM_WORLD);

	//if(rank==0){
		for(i=0;i<N;i++){
			//for(int j=0;j<N;j++){
				printf("C[%d] no rank %d, %f\n",i, rank, C[i]);
			//}
		}
	//}
	MPI_Finalize();
	return 0;
}
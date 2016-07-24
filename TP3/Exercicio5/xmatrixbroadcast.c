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

float **allocate_array(int row_dim, int col_dim) 
{
  float **result;
  int i;

  /* Allocate an array of pointers to hold pointers to the rows of the
	 array */
  result=(float **)malloc(row_dim*sizeof(float *));

  /* The first pointer is in fact a pointer to the entire array */
  result[0]=(float *)malloc(row_dim*col_dim*sizeof(float));

  /* The remaining pointers are just pointers into this array, offset
	 in units of col_dim */
  for(i=1; i<row_dim; i++)
	result[i]=result[i-1]+col_dim;

  return result;
}

void deallocate_array(float **array, int row_dim) 
{
  int i;
  /* Make sure all the pointers into the array are not pointing to
	 random locations in memory */
  for(i=1; i<row_dim; i++)
	array[i]=NULL;
  /* De-allocate the array */
  free(array[0]);
  /* De-allocate the array of pointers */
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

	float **A, **B, **C, **Cpart;
	A = allocate_array(N, N);
	B = allocate_array(N, N);
	C = allocate_array(N, N);
	//Cpart = malloc(sizeof(float)*N*N/nproc);
	Cpart=(float **)malloc((N/nproc)*sizeof(float *));
	/* The first pointer is in fact a pointer to the entire array */
  	Cpart[0]=(float *)malloc((N/nproc)*N*sizeof(float));

  /* The remaining pointers are just pointers into this array, offset
	 in units of col_dim */
  	for(i=1; i<N/nproc; i++)
		Cpart[i]=Cpart[i-1]+N;

	float **recvbufA;
	recvbufA = allocate_array(N/nproc, N);

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
	}
	//processo mestre particiona as matrizes
	MPI_Scatter(A,N*(N/nproc),MPI_FLOAT,recvbufA,N*(N/nproc),MPI_FLOAT,0, MPI_COMM_WORLD);
	MPI_Bcast(&B, N*N, MPI_FLOAT, 0, MPI_COMM_WORLD);

	for(i=0; i<N*N/nproc; i++){
    	for(j=0;j<N; j++){
      		C[i][j]=0.0;
      	}
     }	
     	for (i=0; i<N/nproc; i++){
			for (j=0; j<N; j++){
				printf("recvbufA[%d][%d] %f\n",i, j, recvbufA[i][j]);
			}
		}

	// for(i=0; i<N/nproc; i++){
 //    	for(k=0; k<N; k++){
 //      		for(j=0;j<N; j++){
 //      			C[i][j]+=recvbufA[i][k]*B[k][j];
 //      			// printf("i:%d, k:%d, j:%d\n",i, k, j);
 //      		} 
 //    	}
	// }
	// MPI_Gather(*Cpart,(N/nproc)*N,MPI_FLOAT,*C,(N/nproc)*N,MPI_FLOAT,0, MPI_COMM_WORLD);
	// for(i=0; i<N; i++){
 //      	for(j=0;j<N; j++){
 //      		printf("C[%d][%d] %f\n",i, j, Cpart[i][j]);
 //    	} 
 //    }
	MPI_Finalize();
	return 0;
}
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
	#define N 2
#endif

float *generateRandomMatrix(unsigned long count) {
    float *matrix;	
    float a = 1.0; //número máximo para geração
	matrix = malloc(count*sizeof(float *));
	int i, j;
	for(i=0;i<count;i++){		
		//matrix[i] = ((float)rand()/(float)(RAND_MAX)) * a;				
		matrix[i] = a;				
	}	    
    return matrix;
}

float *allocate_array(unsigned long count) 
{
  float *result;
  result=(float *)malloc(count*sizeof(float));
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
	unsigned long count = (unsigned long) N * (unsigned long)N;
	
	float *recvbufA = (float *)malloc(sizeof(float)*N);
	float *A, *B,*C,*product;		
	A = allocate_array(count);
	B = allocate_array(count);
	C = allocate_array((unsigned long)N);

	if(rank==0){	
		product = allocate_array(count);
		A=generateRandomMatrix(count);
		B=generateRandomMatrix(count);			
	}

	MPI_Scatter(A,N,MPI_FLOAT,recvbufA,N,MPI_FLOAT,0,MPI_COMM_WORLD);			
	MPI_Bcast(B, count, MPI_FLOAT, 0, MPI_COMM_WORLD);

	for(k=0;k<N;k++){
		C[k]=0.0;
		for(i=0;i<N;i++){		
			C[k] += recvbufA[i]*B[k+i*N];			
		}			
	}		
	MPI_Gather(C,N,MPI_FLOAT,product,N,MPI_FLOAT,0,MPI_COMM_WORLD);

	if(rank==0){
		for(i=0;i<count;i++){					
			printf("product[%d]%f\n",i, product[i]);	
		}
	}

	MPI_Finalize();
	return 0;
}
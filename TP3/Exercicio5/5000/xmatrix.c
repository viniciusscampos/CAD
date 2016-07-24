#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
	#define N 500
#endif

float *generateRandomMatrix(unsigned long count) {
    float *matrix;	
    float a = 1000.0; //número máximo para geração
	matrix = malloc(count*sizeof(float *));
	int i, j;
	for(i=0;i<count;i++){		
		matrix[i] = ((float)rand()/(float)(RAND_MAX)) * a;								
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
	struct timeval stop, start;
	float elapsed;	
	gettimeofday(&start,NULL);

	srand(time(NULL));
	int rank, nproc, i, j, k;	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	unsigned long count = (unsigned long) N * (unsigned long)N;
		
	float *A, *B,*C,*product;		
	A = allocate_array(count);
	B = allocate_array(count);	
	unsigned long size = count/nproc;
	C = allocate_array(size);
	float *recvbufA = (float *)malloc(sizeof(float)*size);

	if(rank==0){	
		product = allocate_array(count);
		A=generateRandomMatrix(count);
		B=generateRandomMatrix(count);			
	}

	MPI_Scatter(A,size,MPI_FLOAT,recvbufA,size,MPI_FLOAT,0,MPI_COMM_WORLD);			
	MPI_Bcast(B, count, MPI_FLOAT, 0, MPI_COMM_WORLD);
	
	for(int l=0;l<size/N;l++){
		for(k=0;k<size;k++){
			C[k]=0.0;
			for(i=0;i<N;i++){		
				C[k] += recvbufA[l*N+i]*B[k*(nproc/N)+i*N];			
			}			
		}		
	}	

	MPI_Gather(C,size,MPI_FLOAT,product,size,MPI_FLOAT,0,MPI_COMM_WORLD);

	if(rank==0){
		for(i=0;i<count;i++){					
			printf("product[%d]%f\n",i, product[i]);	
		}
		free(product);		
		gettimeofday(&stop,NULL);
		elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec)/1000.0f;
		printf("Tempo de execução: %f\n", elapsed/1000.0);
	}
	
	free(A);
	free(B);
	free(C);

	MPI_Finalize();		
	return 0;
}
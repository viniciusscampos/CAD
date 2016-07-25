#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
	#define N 1000
#endif

float A[(unsigned long) N * (unsigned long)N];
float B[(unsigned long) N * (unsigned long)N];

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
	unsigned long size = count/nproc;		
	float C[size];
	float product[count];
	float recvbufA[size];	

	if(rank==0){						
		for(int i=0;i<count;i++){
			A[i]=1.0;
			B[i]=1.0;
		}		
	}	
	
	MPI_Scatter(A,size,MPI_FLOAT,recvbufA,size,MPI_FLOAT,0,MPI_COMM_WORLD);			
	MPI_Bcast(B, count, MPI_FLOAT, 0, MPI_COMM_WORLD);

	for(int l=0;l<size/N;l++){
		for(int k=0;k<size;k++){
			C[k]=0.0;
			for(int i=0;i<N;i++){		
				C[k] += recvbufA[l*N+i]*B[k*(nproc/N)+i*N];					
			}						
			//printf("K:%d\n",k);
		}				
	}	

	printf("Meu rank é:%d\n",rank);
	
	MPI_Gather(C,size,MPI_FLOAT,product,size,MPI_FLOAT,0,MPI_COMM_WORLD);

	if(rank==0){
		for(i=0;i<count;i++){					
			//printf("product[%d]%f\n",i, product[i]);	
		}		
		gettimeofday(&stop,NULL);
		elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec)/1000.0f;
		printf("Tempo de execução: %f\n", elapsed/1000.0);
	}
	
	MPI_Finalize();		
	return 0;
}
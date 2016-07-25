#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
	#define N 1000
#endif

#define COUNT ((unsigned long) N*(unsigned long) N)

static int A[COUNT];
static int B[COUNT];
static int product[COUNT];

int main (int argc, char** argv)
{	
	struct timeval stop, start;
	float elapsed;	
	gettimeofday(&start,NULL);	
	int rank, nproc;
				
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);	
	
	unsigned long size = COUNT/nproc;		
	int C[size];
	int recvbufA[size];	
	int aux;

	if(rank==0){								
		for(int i=0;i<COUNT;i++){
			A[i]=1;
			B[i]=1;
		}		
	}	
	
	MPI_Scatter(A,size,MPI_INT,recvbufA,size,MPI_INT,0,MPI_COMM_WORLD);			
	MPI_Bcast(B, COUNT, MPI_INT, 0, MPI_COMM_WORLD);
	
	for(int l=0;l<size/N;l++){
		int NE = l*N;			
		for(int k=0;k<size;k++){
			int knpr= nproc/N;
			C[k]=0.0;
			for(int i=0;i<N;i++){		
				C[k] += recvbufA[NE+i]*B[k*(knpr)+i*N];
			}						
		}				
	}	
		
	MPI_Gather(C,size,MPI_INT,product,size,MPI_INT,0,MPI_COMM_WORLD);

	if(rank==0){
		gettimeofday(&stop,NULL);
		elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec)/1000.0f;				
		printf("Tempo de execução: %f\n", elapsed/1000.0);
	}
		
	MPI_Finalize();		
	return 0;
}
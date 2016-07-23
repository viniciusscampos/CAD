#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, nproc,dest,orig;	
	MPI_Request request;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	if(rank==0){
		dest= nproc-1;
		orig= rank+1;		
	}
	else if(rank==nproc-1){
		dest= rank-1;
		orig= 0;			
	}
	else{
		dest=rank-1;		
		orig= rank+1;
	}	
	float sum=0.0;
	float A = rank;

	for(int i=0;i<nproc;i++){		
		float B;
		MPI_Isend(&A,1,MPI_FLOAT,dest,0,MPI_COMM_WORLD,&request);
		MPI_Irecv(&B,1,MPI_FLOAT,orig,0,MPI_COMM_WORLD,&request);			
		MPI_Wait(&request,&status);
		sum+=B;
		A=B;		
	}

	printf("Sou a tarefa: %d, com soma: %f\n", rank,sum);
	MPI_Finalize();
	return 0;
}
#include "mpi.h"
#include <stdio.h>

int main (argc, argv)
int argc;
char **argv;
{
	MPI_Init(&argc, &argv);
	int rank, nproc, partner,buffer;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	printf("Hello world! Eu sou o processo %d.\n", rank);
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank==0){
		printf("O número total de tarefas é: %d\n", nproc);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank < nproc/2){
		partner = nproc/2 + rank;
	}
	else if (rank >= nproc/2){
		partner = rank - nproc/2;
	}

	buffer = rank;	
	MPI_Send(&buffer,1,MPI_INT,partner,0,MPI_COMM_WORLD);
	MPI_Recv(&buffer,1,MPI_INT,partner,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	printf("A tarefa %d tem parceira %d.\n", rank,partner);
	
	MPI_Finalize();
	return 0;
}
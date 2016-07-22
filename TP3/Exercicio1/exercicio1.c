#include "mpi.h"
#include <stdio.h>

int main (argc, argv)
int argc;
char **argv;
{
	MPI_Init(&argc, &argv);
	int rank, nproc;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	printf("Hello world! Eu sou o processo %d.\n", rank);
	if(rank==0){
		printf("O número total de tarefas é: %d\n", nproc);
	}
	MPI_Finalize();
	return 0;
}
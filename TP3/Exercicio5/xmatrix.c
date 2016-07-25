#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define N 1000

#define COUNT ((unsigned long) N*(unsigned long) N)

int A[N][N],B[N][N],C[N][N];

void print_matrix(int m[N][N])
{
    int i, j = 0;
    for (i=0; i<N; i++) {
        printf("\n\t| ");
        for (j=0; j<N; j++)
            printf("%2d ", m[i][j]);
        printf("|");
    }
}

int main(int argc, char *argv[])
{   
    struct timeval stop, start;
    float elapsed;  
    gettimeofday(&start,NULL);  
    int rank, nproc,from,to;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);


    if (N%nproc!=0) {
        if (rank==0) printf("Matrix size not divisible by number of processors\n");
        MPI_Finalize();
        exit(-1);
    }

    from = rank * N/nproc;
    to = ((rank+1) * N/nproc);
    
    if(rank==0){
        for(int i=0;i<N;i++){
          for(int j=0;j<N;j++){      
            A[i][j] = 1;
            B[i][j] = 1;
          }
        }
    }
    
    MPI_Bcast (B, COUNT, MPI_INT, 0, MPI_COMM_WORLD);    
    MPI_Scatter (&A[0][0], COUNT/nproc, MPI_INT, &A[from][0], N*N/nproc, MPI_INT, 0, MPI_COMM_WORLD);
    
    for(int i=from; i<to; i++){
        for(int j=0; j<N; j++) {
            C[i][j]=0;
            for(int k=0; k<N; k++){
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }         

    MPI_Gather(&C[from][0], COUNT/nproc, MPI_INT, &C[from][0], COUNT/nproc, MPI_INT, 0, MPI_COMM_WORLD);  

    if (rank==0) {
    	/*
        printf("\n\n\t       = \n");
        print_matrix(C);
        printf("\n\n");
        */
		gettimeofday(&stop,NULL);
		elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec)/1000.0f;
		printf("Tempo de execução: %f\n", elapsed/1000.0);

    }

    MPI_Finalize();
    return 0;
}
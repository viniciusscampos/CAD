#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
  #define N 10000000
#endif

float A[N],B[N];

int main (int argc, char** argv)
{
  struct timeval stop, start;
  float elapsed;  
  gettimeofday(&start,NULL);

  MPI_Init(&argc, &argv);
  int rank, nproc,from,to;
  double globalsum,localsum = 0.0;  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  int local_N = N/nproc;
  srand(time(NULL)); 

  from = rank* local_N;
  to = ((rank+1) * N/nproc);

  int i;
  for (i=0; i<N; i++){
    A[i]=1.0;
    B[i]=1.0;
  }
  MPI_Scatter(&A[0],local_N,MPI_FLOAT,&A[from],local_N,MPI_FLOAT,0,MPI_COMM_WORLD);
  MPI_Scatter(&B[0],local_N,MPI_FLOAT,&B[from],local_N,MPI_FLOAT,0,MPI_COMM_WORLD);


  for(i=from;i<to;i++){
    localsum =  localsum + A[i]*B[i];
    // printf("Local sum do rank %d é igual a %f\n",rank, localsum);
    // printf("A: %f\n", recvbufA[i]);
    // printf("B: %f\n", recvbufB[i]);
  }   
  
  MPI_Reduce(&localsum,&globalsum,1,MPI_DOUBLE,MPI_SUM, 0, MPI_COMM_WORLD);

  if(rank==0){
    printf("Global sum: %f\n", globalsum);
    gettimeofday(&stop,NULL);
    elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec)/1000.0f;
    printf("Tempo de execução: %f\n", elapsed/1000.0);      
  } 

  MPI_Finalize();
  return 0;
}
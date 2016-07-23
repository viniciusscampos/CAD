#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
  #define N 8
#endif

float generateRandomNumber (){
  int i;
  float a = 5.0; //número máximo para geração
  return ((float)rand()/(float)(RAND_MAX)) * a;
 
}

int main (int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  int rank, nproc;
  float globalsum = 0.0, localsum = 0.0;  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  int local_N = N/nproc;
  srand(time(NULL)); 
  
  float *A;
  float *B;
  A = malloc(N*sizeof(float));
  B = malloc(N*sizeof(float));

  float recvbufA[N/nproc];
  float recvbufB[N/nproc];

  int i;
  for (i=0; i<N; i++){
    A[i]=generateRandomNumber();
    B[i]=generateRandomNumber();
  }
  MPI_Scatter(A,local_N,MPI_FLOAT,recvbufA,local_N,MPI_FLOAT,0,MPI_COMM_WORLD);
  MPI_Scatter(B,local_N,MPI_FLOAT,recvbufB,local_N,MPI_FLOAT,0,MPI_COMM_WORLD); 


  for(i=0;i<local_N;i++){
    localsum =  localsum + recvbufA[i]*recvbufB[i];
    printf("Local sum do rank %d é igual a %f\n",rank, localsum);
    printf("A: %f\n", recvbufA[i]);
    printf("B: %f\n", recvbufB[i]);
  }   
  
  MPI_Reduce(&localsum,&globalsum,1,MPI_FLOAT,MPI_SUM, 0, MPI_COMM_WORLD);
  if(rank==0){
    printf("Global sum: %f\n", globalsum);
  } 
  free(A);
  free(B);
  MPI_Finalize();
  return 0;
}
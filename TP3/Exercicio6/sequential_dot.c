#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
  #define N 12
#endif

float generateRandomNumber (){
  int i;
  float a = 5.0; //número máximo para geração
  return ((float)rand()/(float)(RAND_MAX)) * a;
}

int main (int argc, char** argv)
{
  srand(time(NULL)); 
  int i;
  float sum=0;
  float *A;
  float *B;
  A = malloc(N*sizeof(float));
  B = malloc(N*sizeof(float));

  for (i=0; i<N; i++){
    A[i]=generateRandomNumber();
    B[i]=generateRandomNumber();
  }

  for (i = 0; i < N; i++) {
    sum += A[i] * B[i];
  }
  printf("Soma: %f\n", sum);
 
  free(A);
  free(B);
  return 0;
}
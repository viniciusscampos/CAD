#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
  #define N 100000000
#endif

float generateRandomNumber (){
  int i;
  //float a = 5.0; //número máximo para geração
  //return ((float)rand()/(float)(RAND_MAX)) * a;
  return 1.0;
}

int main (int argc, char** argv)
{
  struct timeval stop, start;
  float elapsed;  
  gettimeofday(&start,NULL);

  srand(time(NULL)); 
  int i;
  double sum=0;
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

  gettimeofday(&stop,NULL);
  elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec)/1000.0f;
  printf("Tempo de execução: %f\n", elapsed/1000.0);  
  return 0;
}
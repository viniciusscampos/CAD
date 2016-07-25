#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifndef N
  #define N 10000000
#endif

float A[N],B[N];
double sum=0;

int main (int argc, char** argv)
{
  struct timeval stop, start;
  float elapsed;  
  gettimeofday(&start,NULL);

  srand(time(NULL)); 
  int i;

  for (i=0; i<N; i++){
    A[i]=1.0;
    B[i]=1.0;
  }

  for (i = 0; i < N; i++) {
    sum += A[i] * B[i];
  }
  printf("Soma: %f\n", sum);

  gettimeofday(&stop,NULL);
  elapsed = (stop.tv_sec - start.tv_sec)*1000.0f + (stop.tv_usec - start.tv_usec)/1000.0f;
  printf("Tempo de execução: %f\n", elapsed/1000.0);  
  return 0;
}
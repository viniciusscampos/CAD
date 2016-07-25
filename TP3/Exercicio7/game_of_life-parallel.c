/***********************

Jogo da Vida
Versão sequencial 

************************/
#include "mpi.h"
#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#define NI 0        /* tamanho dos array  */
#define NJ 0

#define NSTEPS 500    /* Numero de iteracoes */

int old[NI][NJ],new[NI][NJ],globalsum=0;

int main(int argc, char *argv[]) 
{

  int rank, nproc; 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  int i, j, n, im, ip, jm, jp, ni, nj, nsum, isum,size,from,to;
  float x;  
  /* alocacao */

  ni = NI + 2;  /* celulas fantasmas na borda  */
  nj = NJ + 2;
  size = nj*ni/nproc;

  if(rank==0){
    for(int i=0;i<NI;i++){
      for(int j=0;j<NJ;j++){      
        old[i][j] = 1;
      }
    }
  }
  
  from = rank * ni/nproc;
  to = ((rank+1) * ni/nproc);

  MPI_Scatter(&(old[0][0]),size,MPI_INT,&old[from][0],size,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Scatter(&(new[0][0]),size,MPI_INT,&new[from][0],size,MPI_INT,0,MPI_COMM_WORLD);  
  
  for(int i=0;i<size;i++){      
    printf("Rank: %d , %d\n", rank,old[from][i]);      
  }

  /* */
  for(n=0; n<NSTEPS; n++)
  {

    /* condicoes de controno para as esquinas do dominio */
    int first,last;
    first=0;
    last = (nproc-1) * ni/nproc;

    old[first][0]       = old[last][NJ];
    old[first][NJ+1]    = old[last][1];
    old[last+1][NJ+1] = old[first+1][1];
    old[last+1][0]    = old[first+1][NJ];

    /* cond. contorno para faces direita/esquerda  */

    for(i=from; i<=to; i+=NJ){                
      old[i][0]    = old[i][NJ];
      old[i][NJ+1] = old[i][1];
    }

    /* cond. controno face inferior e superior */

    if(rank==0){
      for(j=1; j<=NJ; j++){
        //up
        old[from][j] = old[to][j];
        old[from+1][j] = old[to+1][j];
        //last
        int last= (nproc-1) * ni/nproc;
        old[from][j] = old[last][j];
        old[from+1][j] = old[last+1][j];
      }
    }
    else if(rank==nproc-1){
      for(j=1; j<=NJ; j++){
        //first
        int first=0;
        old[from][j] = old[first][j];
        old[from+1][j] = old[first+1][j];
        //down
        int previous = (rank-1) * ni/nproc;        
        old[from][j] = old[previous][j];
        old[from+1][j] = old[previous+1][j];
      }
    }        
    else{
        for(j=1; j<=NJ; j++){
        //up
        old[from][j] = old[to][j];
        old[from+1][j] = old[to+1][j];
        //down
        int previous = (rank-1) * ni/nproc;        
        old[from][j] = old[previous][j];
        old[from+1][j] = old[previous+1][j];
        }
    }

    // Codigo Paralelo: Trocar elementos da interface paralela

    for(i=from; i<=to; i+=NJ)
    {
      for(j=1; j<=NJ; j++)
      {
        im = i-1;
    	  ip = i+1;
    	  jm = j-1;
    	  jp = j+1;

      nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
            + old[im][j ]              + old[ip][j ] 
            + old[im][jm] + old[i][jm] + old[ip][jm];

      printf("%d\n", nsum);
    	  switch(nsum)
        {
          case 3:
            new[i][j] = 1;
            break;
          case 2:
            new[i][j] = old[i][j];
            break;
          default:
            new[i][j] = 0;
    	  }
       }
    }

    /* copia estado  */
    for(i=from; i<=to; i+=NJ){
      for(j=1; j<=NJ; j++){
        old[i][j] = new[i][j];
        //printf("%d\n",new[i][j] );
        //printf("i=%d,j=%d,%d\n",i,j,isum );
      }
    }
  }

  /*  Conta o número de celulas  vivas no final */
  isum = 0;
  for(i=from; i<=to; i+=NJ){
    for(j=1; j<=NJ; j++){
      isum = isum + new[i][j];      
    }
  }  

  MPI_Reduce(&isum,&globalsum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

      
  FILE *outfile;
  char outfilename[128];  
  char result[256];
  sprintf(outfilename, "found.data_%d", rank);  
  outfile = fopen(outfilename, "w");
  sprintf(result,"# Celulas Vivas = %d\n", globalsum);  
  fprintf(outfile, "%s\n", result);
  fclose(outfile);

  /*for(i=0; i<ni; i++){
    free(old[i]); 
    free(new[i]); 
  }

  free(old);
  free(new);*/
  
  MPI_Finalize();
  return 0;
}

/***********************

Jogo da Vida
Versão sequencial 

************************/
#include "mpi.h"
#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#define NI 100        /* tamanho dos array  */
#define NJ 100

#define NSTEPS 500    /* Numero de iteracoes */
#define np 2

int old[NI+2*np][NJ+2*np],new[NI][NJ],globalsum=0;

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
  size = (nj*ni)/nproc;

  if(rank==0){    
    for(i=1; i<=NI; i++)
      {
        for(j=1; j<=NJ; j++)
        {
          old[i][j]=1;
        }
      }
  }
  

  // if(rank==nproc-1){
  //   from = rank * ni/nproc;
  //   to = 0;
  // }
  //else{
    from = rank * ni/nproc;
    to = ((rank+1) * ni/nproc);
  //}  

  MPI_Scatter(&(old[0][0]),size,MPI_INT,&old[from][0],size,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Scatter(&(new[0][0]),size,MPI_INT,&new[from][0],size,MPI_INT,0,MPI_COMM_WORLD);  
  
  // for(int i=0;i<size;i++){      
  //   //printf("Rank: %d ,%d, %d\n", rank,i,old[from][i]);      
  // }

  /* */
  for(n=0; n<NSTEPS; n++)
  {

    /* condicoes de controno para as esquinas do dominio */
    int first,last;
    first=0;
    last = (nproc-1) * ni/nproc;

    old[first][0]       = old[last][NJ];
    old[first][NJ+1]    = old[last][1];
    old[last+NI+1][NJ+1] = old[first+1][1];
    old[last+NI+1][0]    = old[first+1][NJ];

    /* cond. contorno para faces direita/esquerda  */

    for(i=from; i<=ni/nproc; i++){                
      old[i][0]    = old[i][NJ];
      old[i][NJ+1] = old[i][1];
    }

    /* cond. controno face inferior e superior */    
    if(rank==0){
      //printf("%d\n", nj);
      int last= (nproc-1) * ni/nproc;
      for(j=1; j<=nj; j++){
        //printf("%d\n", j);
        //printf("%d\n",sizeof(old[from])/sizeof(int) );
        //printf("(%d)\n", old[to+1][55]);
        //printf("(%d)\n", to);
        //top, from+NI e o topo do pedaco de from
        old[from+NI][j] = old[to-2][j];
        old[from+NI+1][j] = old[to-1][j];
        //bottom
        old[from][j] = old[last+NI][j];
        old[from+1][j] = old[last+NI+1][j];
      }
    }    
     else if(rank==nproc-1){
       for(j=1; j<=nj; j++){
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
        int previous = (rank-1) * ni/nproc;
        for(j=1; j<=nj; j++){
        //up
        old[from][j] = old[to-2][j];
        old[from+1][j] = old[to-1][j];
        //down/                
        old[from][j] = old[previous][j];
        old[from+1][j] = old[previous+1][j];
        }
    }

    // Codigo Paralelo: Trocar elementos da interface paralela

    for(i=from+1; i<=NI/nproc; i++)
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
      
      //printf("%d\n",nsum );
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
    for(i=from+1; i<=ni/nproc; i++){
      for(j=1; j<=nj; j++){
        old[i][j] = new[i][j];
        //printf("%d\n",new[i][j] );
        //printf("i=%d,j=%d,%d\n",i,j,isum );
      }
    }
  }

  /*  Conta o número de celulas  vivas no final */
  isum = 0;
  for(i=from+1; i<=ni/nproc; i++){
    for(j=1; j<=nj; j++){
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

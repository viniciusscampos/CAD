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

int old[NI+2][NJ+2],new[NI+2][NJ+2];

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
    for(int i=0;i<ni;i++){
      for(int j=0;j<ni;j++){      
        old[i][j] = 1;
        new[i][j] = 1;
      }
    }
  }
  
  from = rank * ni/nproc;
  to = ((rank+1) * ni/nproc);

  MPI_Scatter(&(old[0][0]),size,MPI_INT,&old[from][0],size,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Scatter(&(new[0][0]),size,MPI_INT,&new[from][0],size,MPI_INT,0,MPI_COMM_WORLD);
  
  //MPI_Scatter(new,size,MPI_INT,recvbufNew,size,MPI_INT,0,MPI_COMM_WORLD);
  
    for(int i=0;i<ni;i++){
      for(int j=0;j<ni;j++){
        printf("Rank: %d , %d\n", rank,old[i][j]);
      }      
    }

 //  /* */
 //  for(n=0; n<NSTEPS; n++)
 //  {

 //    /* condicoes de controno para as esquinas do dominio */
 //    old[0][0]       = old[NI][NJ];
 //    old[0][NJ+1]    = old[NI][1];
 //    old[NI+1][NJ+1] = old[1][1];
 //    old[NI+1][0]    = old[1][NJ];

 //    /* cond. contorno para faces direita/esquerda  */

 //    for(i=1; i<=NI; i++){
 //      old[i][0]    = old[i][NJ];
 //      old[i][NJ+1] = old[i][1];
 //    }

 //    /* cond. controno face inferior e superior */

 //    for(j=1; j<=NJ; j++){
 //      old[0][j]    = old[NI][j];
 //      old[NI+1][j] = old[1][j];
 //    }

 //    // Codigo Paralelo: Trocar elementos da interface paralela

 //    for(i=1; i<=NI; i++)
 //    {
 //       for(j=1; j<=NJ; j++)
 //       {
	//   im = i-1;
	//   ip = i+1;
	//   jm = j-1;
	//   jp = j+1;

	//   nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
	//         + old[im][j ]              + old[ip][j ] 
	//         + old[im][jm] + old[i][jm] + old[ip][jm];

	//   switch(nsum)
 //          {
 //             case 3:
	//         new[i][j] = 1;
	//      break;
	//      case 2:
	//         new[i][j] = old[i][j];
	//      break;
 //             default:
	//        new[i][j] = 0;
	//   }
 //       }
 //    }

 //    /* copia estado  */
 //    for(i=1; i<=NI; i++){
 //      for(j=1; j<=NJ; j++){
	// old[i][j] = new[i][j];
 //      }
 //    }
 //  }

 //  /*  Conta o número de celulas  vivas no final */
 //  isum = 0;
 //  for(i=1; i<=NI; i++){
 //    for(j=1; j<=NJ; j++){
 //      isum = isum + new[i][j];
 //    }
 //  }
  
 //  FILE *outfile;
 //  char outfilename[128];  
 //  char result[256];
 //  sprintf(outfilename, "found.data_%d", rank);  
 //  outfile = fopen(outfilename, "w");
 //  sprintf(result,"# Celulas Vivas = %d\n", isum);  
 //  fprintf(outfile, "%s\n", result);
 //  fclose(outfile);

 //  for(i=0; i<ni; i++){
 //    free(old[i]); 
 //    free(new[i]); 
 //  }

 //  free(old);
 //  free(new);
  
  MPI_Finalize();
  return 0;
}

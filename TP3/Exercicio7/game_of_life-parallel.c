/***********************

Jogo da Vida
Versão sequencial 

************************/

#include <mpi.h>
#include <stdio.h>

#include <stdlib.h>

#define NI 700        /* tamanho dos array  */
#define NJ 700

#define NSTEPS 500    /* Numero de iteracoes */

int main(int argc, char *argv[]) 
{

  MPI_Init(&argc,&argv);

  int rank,nproc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);     

  int i, j, n, im, ip, jm, jp, ni, nj, nsum, isum,ri,rj,top,down,globalsum;
  int **old, **new;  
  float x;

  /* alocacao */

  ni = (NI/nproc) + 2;  /* celulas fantasmas na borda  */
  nj = NJ + 2;

  /*top e down sao os processos cujas matrizes estao acima e abaixo
    o processo deve se comunicar com esses processos para verificar as fronteiras
    ultimo processo */
  if(rank == nproc -1){
    // recebe o total menos as partes ja distribuidas aos demais processos
    ni = 2 + NI - (rank*(NI/nproc));
    top = 0;
    down = rank-1;
  }
  else if(rank==0){
    top = 1;
    down = nproc-1;
  }
  else{
    top = rank+1;
    down = rank-1;
  }


  //numero de linhas e colunas nao fantasmas da matriz
  ri = ni -2;
  rj = nj -2;

  old = malloc(ni*sizeof(int*));
  new = malloc(ni*sizeof(int*));

  for(i=0; i<ni; i++){
    old[i] = malloc(nj*sizeof(int));
    new[i] = malloc(nj*sizeof(int));
  }

/*  inicializando elementos  */

  for(i=1; i<=ri; i++)
  {
    for(j=1; j<=rj; j++)
    {
       x = rand()/((float)RAND_MAX + 1);
       if(x<0.5){
    old[i][j] = 0;
       } else {
    old[i][j] = 1;
       }
    }
  }

  /* */
  for(n=0; n<NSTEPS; n++)
  {

    /* condicoes de contorno para as esquinas do dominio */

    if(rank==0){
      old[0][0] = old[ri][rj];
      old[0][rj+1] = old[ri][1];
    }
    if(rank==nproc-1){
      old[ri+1][rj+1] = old[1][1];
      old[ri+1][0] = old[1][rj];
    }

    MPI_Request request[4];
    MPI_Status status[4];
    
    /* cond. contorno para faces direita/esquerda  
    Como estamos programando em C e nosso dominio esta sendo
    subdividido em linhas nao precisamos trocar mensagens para obter
    os elementos que precisamos nessa etapa*/
    for(i=1; i<=ri; i++){
      old[i][0]    = old[i][rj];
      old[i][rj+1] = old[i][1];
    }

    /* cond. controno face inferior e superior */    
    /*Envio para o processo acima (top) a minha ultima linha,superior,nao fantasma
      e recebo dele a linha dele nao fantasma a qual deverei sobrepor
      a minha linha fantasma superior*/
    MPI_Isend(old[ri],rj,MPI_INT,top,0,MPI_COMM_WORLD,&request[0]);
    MPI_Irecv(old[ri+1],rj,MPI_INT,top,0,MPI_COMM_WORLD,&request[1]);

    /*Envio para o processo abaixo (down) a minha primeira linha,inferior,nao fantasma
      e recebo dele a linha dele nao fantasma a qual deverei sobrepor
      a minha linha fantasma superior*/
    //old[1] contem a primeira linha nao fantasma da matriz do processo
    MPI_Isend(old[1],rj,MPI_INT,down,0,MPI_COMM_WORLD,&request[2]);
    //old[0] contem a primeira linha fantasma da matriz do processo
    MPI_Irecv(old[0],rj,MPI_INT,down,0,MPI_COMM_WORLD,&request[3]);    
    

    // Codigo Paralelo: Trocar elementos da interface paralela
    /*Deve analisar as linhas e as colunas nao fantasmas, ou seja,
      que nao dependem dos vizinhos*/
    for(i=2; i<=ri; i++)
    {
       for(j=1; j<=rj+1; j++)
       {
    im = i-1;
    ip = i+1;
    jm = j-1;
    jp = j+1;

    nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
          + old[im][j ]              + old[ip][j ] 
          + old[im][jm] + old[i][jm] + old[ip][jm];

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
    /*Antes de calcularmos as linhas que dependem dos vizinhos
      devemos possuir todos os valores necessarios*/
    MPI_Waitall(4,request,status);
    /*Para o calculo das linhas que dependem dos vizinhos
    dividiremos em dois blocos. Um deles para o do vizinho
    de cima (top) e outro para o vizinho de baixo (down)*/
   //Calculo para as linhas que dependem do vizinho de cima (top)

    i = 1;
    for(j=1; j<=rj+1; j++)
    {
      im = i-1;
      ip = i+1;
      jm = j-1;
      jp = j+1;

      nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
            + old[im][j ]              + old[ip][j ] 
            + old[im][jm] + old[i][jm] + old[ip][jm];

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

 //Calculo para as linhas que dependem do vizinho de baixo (down)

    i = ri;
    for(j=1; j<=rj+1; j++)
    {
      im = i-1;
      ip = i+1;
      jm = j-1;
      jp = j+1;

      nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
            + old[im][j ]              + old[ip][j ] 
            + old[im][jm] + old[i][jm] + old[ip][jm];

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

    /* copia estado  */
    for(i=1; i<=ri; i++){
      for(j=1; j<=rj; j++){
        old[i][j] = new[i][j];
      }
    }
  }

  /*  Conta o número de celulas  vivas no final */
  isum = 0;
  for(i=1; i<=ri; i++){
    for(j=1; j<=rj; j++){
      isum = isum + new[i][j];
    }
  }
  //printf("\n# Celulas Vivas = %d\n", isum);

  MPI_Reduce(&isum,&globalsum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

  for(i=0; i<ni; i++){
    free(old[i]); 
    free(new[i]); 
  }

  free(old);
  free(new);

  FILE *outfile;
  char outfilename[128];  
  char result[256];
  sprintf(outfilename, "found.data_%d", rank);  
  outfile = fopen(outfilename, "w");
  if(rank==0){
    fprintf(outfile, "Olá eu sou a tarefa de rank: %d\n", rank);
    fprintf(outfile, "O número total de processos é: %d\n", nproc);
    sprintf(result,"# Celulas Vivas = %d\n", globalsum);  
    fprintf(outfile, "%s\n", result);    
  }
  else{
    fprintf(outfile, "Olá eu sou a tarefa de rank: %d\n", rank);    
    fprintf(outfile, "%s\n", result);      
  }
  fclose(outfile);


  MPI_Finalize();

  return 0;
}

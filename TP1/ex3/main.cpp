#include <string>
#include <iostream>
#include <cstdlib>
#include <chrono> 
#include <fstream>

using namespace std;

#ifndef N
	#define N 1000
#endif

int main(int argc, char** argv) {

	//getting time for the start of the program
	auto firstTime = chrono::steady_clock::now();

	string tipo;

	srand(time(NULL));

	int **A = new int*[N];
	int **B = new int*[N];
	int **C = new int*[N];

	//inicialize the matrix A and B with random values with range N
	for(int i=0; i<N; i++){
		A[i] = new int[N];
		B[i] = new int[N];
		C[i] = new int[N];
		for(int j=0; j<N; j++){
			int resultRandomNumber = (rand() % N);
			A[i][j] = resultRandomNumber;
			B[i][j] = resultRandomNumber;
		}
	}	

	//getting time for the start of activity
	auto secondTime = chrono::steady_clock::now();

	//matrix multiplication with normal approach
	#ifndef INVERTED
		tipo = "n";
		for(int j=0; j< N; j++){
			for(int i=0; i<N; i++){
				C[i][j] = 0;
				for(int k=0; k<N; k++){
					C[i][j] += C[i][j] + A[i][k]*B[k][j];
				}
			}
		}

	#else
		//matrix multiplication with inverted approach
		tipo = "i";
		#ifndef BLOCK
		for(int i=0; i< N; i++){
			for(int j=0; j<N; j++){
				C[i][j] = 0;
				for(int k=0; k<N; k++){
					C[i][j] += C[i][j] + A[j][k]*B[k][i];
				}
			}
		}
		#else
		for(int i=0; i< N; i+= BLOCK){
			for(int j=0; j<N; j+= BLOCK){
				C[i][j] = 0;
				for(int k=0; k<N; k+=BLOCK){
					const int imax = min(N,i+BLOCK);
					const int jmax = min(N,j+BLOCK);
					const int kmax = min(N,k+BLOCK);
					for(int ii=i; ii<imax; ii++){
						for(int jj = j; jj<jmax; jj++){
							for(int kk=k; kk<kmax; kk++){
								C[ii][jj]= C[ii][jj] + A[jj][kk]*B[kk][ii];
							}
						}
					}
				}
			}
		}	
		#endif	
	#endif


	//get the time for the end of the activity
	auto lastTime = chrono::steady_clock::now();
	delete[] A;
	delete[] B;
	delete[] C;
	#ifndef BLOCK
		string fileName = to_string(N) +"-"+tipo+".txt";
	#else
		string fileName = to_string(N) +"-"+tipo+ "-" + to_string(BLOCK) +".txt";
	#endif
	fstream fs;
	fs.open(fileName, fstream::in | fstream::out | fstream::app);
	fs << "Tempo de execução total: " <<std::chrono::duration<double, std::milli>(lastTime - firstTime).count() << endl;
	fs << "Tempo de multiplicação: " <<std::chrono::duration<double, std::milli>(lastTime - secondTime).count() << endl;
	fs.close();
	return 0;
}
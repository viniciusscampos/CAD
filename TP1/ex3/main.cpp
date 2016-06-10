#include <string>
#include <iostream>
#include <cstdlib>
#include <chrono> 
#include <fstream>

using namespace std;

int main(int argc, char** argv) {

	//getting time for the start of the program
	auto firstTime = chrono::steady_clock::now();

	//catching the matrix size
	const string argument1 = argv[1];
	int N = stoi(argument1);
	
	const string argument2 = argv[2];	

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
	if(argument2 == "n"){
		for(int j=0; j< N; j++){
			for(int i=0; i<N; i++){
				C[i][j] = 0;
				for(int k=0; k<N; k++){
					C[i][j] += C[i][j] + A[i][k]*B[k][j];
				}
			}
		}
	}
	//matrix multiplication with inverted approach
	else if(argument2 == "i"){
		for(int i=0; i< N; i++){
			for(int j=0; j<N; j++){
				C[i][j] = 0;
				for(int k=0; k<N; k++){
					C[i][j] += C[i][j] + A[j][k]*B[k][i];
				}
			}
		}
	}


	//get the time for the end of the activity
	auto lastTime = chrono::steady_clock::now();
	delete[] A;
	delete[] B;
	delete[] C;
	string fileName = argument1+"-"+argument2+".txt";
	fstream fs;
	fs.open(fileName, fstream::in | fstream::out | fstream::app);
	fs << "Tempo de execução total: " <<std::chrono::duration<double, std::milli>(lastTime - firstTime).count() << endl;
	fs << "Tempo de multiplicação: " <<std::chrono::duration<double, std::milli>(lastTime - secondTime).count() << endl;
	fs.close();
	return 0;
}
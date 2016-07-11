#include <iostream>
#include <vector> 
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>

#ifndef N
    #define N 1000000
#endif

#ifndef NT
    #define NT 4
#endif


std::vector<std::vector <double> > generateRandomVector() {
    srand(time(NULL));
    std::vector<std::vector<double> > ret;
    ret.reserve(N);
    for (unsigned long int i = 0; i < N; i++){
        double x = (double) rand() / ((double) RAND_MAX +1);
        double y = (double) rand() / ((double) RAND_MAX +1);
        double lt[] = {x,y};
        std::vector<double> vec (lt, lt + sizeof(lt) / sizeof(lt[0]) );
        ret.push_back(vec);
    }
    return ret;
}


int main(int argc, char** argv) {

    double M = 0;
    std::vector<std::vector <double> > aux = generateRandomVector();
    
    omp_set_num_threads(NT);    
    #pragma omp parallel
    {
        std::cout<<omp_get_num_threads()<<","<<std::endl;
        #pragma omp for schedule(static) reduction(+:M)
        for (int i=0; i< N;i++){
           double rx = aux[i][0] - 0.5; //d² = (Px - Cx)² + (Py - Cy)²
           double ry = aux[i][1] - 0.5;
           double d = std::sqrt( std::pow(rx,2) + std::pow(ry,2) );           
           if(d< 0.5){     
               M+=1;
           }
        }
    }     
    std::cout << (4*M)/N << std::endl;
    
}


/*
EXEMPLO DE COMPILAÇÃO

g++ -std=c++11 -fopenmp -DN=10000000 -DNT=3 main.cpp -o main


*/


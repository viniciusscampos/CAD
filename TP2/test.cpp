#include <iostream>
#include <vector> 
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>


int main(int argc, char** argv) {

    
    omp_set_num_threads(4);    
    #pragma omp parallel sections
    {       
      std::cout<<omp_get_num_threads()<<std::endl; 
    }   
    
}
#include <iostream>
#include <vector> 
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <string>
#include <omp.h>

#ifndef NT
    #define NT 2
#endif


std::vector <int> generateRandomVector(int N) {
    srand(time(NULL));
    std::vector<int> ret;    
    ret.reserve(N);    
    for (unsigned long int i = 0; i < N; i++){  
      int x = rand();
        ret.push_back(x);
    }
    return ret;
}

int partition(std::vector<int> &vec, int begin, int end) {
    const int mid = begin + (end - begin) / 2;
    const int pivot = vec[mid];    
    std::swap(vec[mid],vec[begin]);
    int i = begin + 1;
    int j = end;

    while (i <= j) {
        while(i <= j && vec[i] <= pivot) {
            i++;
        }

        while(i <= j && vec[j] > pivot) {
            j--;
        }

        if (i < j) {
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i - 1],vec[begin]);
    return i - 1;
}

void quicksort(std::vector<int> &vec, int begin, int end, int size){
    if (begin >= end) {
        return;
    }

    int part = partition(vec, begin, end);

    #pragma omp parallel sections num_threads(NT)
    {       
      #pragma omp section
        {
          quicksort(vec, begin, part - 1, size);
        }
      #pragma omp section
        {         
          quicksort(vec, part + 1, end, size);
        }    
    }    
}

int main(int argc, char** argv) {
  auto startTime = std::chrono::steady_clock::now();
  const std::string argument1 = argv[1];
  int K = std::stoi(argument1);

  std::vector<int> vec = generateRandomVector(std::pow(2,K));
  quicksort(vec, 0, vec.size() - 1, vec.size());
  auto endTime = std::chrono::steady_clock::now();
  
  std::cout << std::chrono::duration<double, std::milli>(endTime - startTime).count() << std::endl;
}
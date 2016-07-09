#include <iostream>
#include <vector> 
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>


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

void quickSort(std::vector<int> &arr, int left, int right) {
      int i = left, j = right;
      int tmp;
      int pivot = arr[(left + right) / 2];
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
      /* recursion */
      if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
}

int main(int argc, char** argv) {
  std::vector<int> arr = generateRandomVector(10);
  quickSort(arr, 0, arr.size()-1);
  for(int tr: arr){
  	std::cout<< tr << std::endl;
  }
}
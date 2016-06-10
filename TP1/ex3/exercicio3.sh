#!/bin/bash

#compile the code with diferents levels of optimization
for o in O1 O2 O3
do
    mkdir main-$o
    cd main-$o
    g++ -$o -std=c++11 main.cpp -o main$o
    cd ../
done

#run the program with diferents values for N, diferent levels of optimization 
#and alternating the normal and inverted loop order
for o in O1 O2 O3
do
    cd main-$o
    for N in 1000 2000 3000 4000 5000
    do
        for k in n i        
        do            
            ./main$o $N $k
        done
    done
    cd ../
done

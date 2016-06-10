
#!/bin/bash

RUN_PATH="/home/vinicius/local/hpctoolkit/bin"

mkdir block-i
cd block-i

for o in O1 O2 O3
do
	for N in 8 16 32 64 128 256 512
	do
	    mkdir 2000-$N-$o
	    cd 2000-$N-$o
	      g++ -$o -std=c++11 -DN=2000 -DINVERTED -DBLOCK=$N ../../main.cpp -o main2000$N$o
	      ./main2000$N$o
	    cd ../
	done
done
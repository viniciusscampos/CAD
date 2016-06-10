
#!/bin/bash

RUN_PATH="/home/vinicius/local/hpctoolkit/bin"


for N in 4000000 8000000 16000000 32000000 64000000 128000000
do
    mkdir stream-$N
    cd stream-$N
      gcc -g -O -DSTREAM_ARRAY_SIZE=$N ../../stream.c -o stream$N
    cd ../
done

for N in 4000000 8000000 16000000 32000000 64000000 128000000
do
    cd stream-$N
    $RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./stream$N
    cd ../
done

for N in 4000000 8000000 16000000 32000000 64000000 128000000
do
    cd stream-$N
    $RUN_PATH/hpcstruct ./stream$N
    $RUN_PATH/hpcprof -S stream$N.hpcstruct -I~ hpctoolkit-stream$N-measurements
    cd ../
done

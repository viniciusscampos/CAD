
#!/bin/bash

RUN_PATH="/home/vinicius/local/hpctoolkit/bin"

mkdir hpc-n
cd hpc-n

echo "running n -O1"
g++ -O1 -std=c++11 -DN=4000 ../main.cpp -o main4000no1
$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./main4000no1
$RUN_PATH/hpcstruct ./main4000no1
$RUN_PATH/hpcprof -S main4000no1.hpcstruct -I~ hpctoolkit-main4000no1-measurements

echo "running n -O2"
g++ -O2 -std=c++11 -DN=4000 ../main.cpp -o main4000no2
$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./main4000no2
$RUN_PATH/hpcstruct ./main4000no2
$RUN_PATH/hpcprof -S main4000no2.hpcstruct -I~ hpctoolkit-main4000no2-measurements

echo "running n -O3"
g++ -O3 -std=c++11 -DN=4000 ../main.cpp -o main4000no3
$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./main4000no3
$RUN_PATH/hpcstruct ./main4000no3
$RUN_PATH/hpcprof -S main4000no3.hpcstruct -I~ hpctoolkit-main4000no3-measurements


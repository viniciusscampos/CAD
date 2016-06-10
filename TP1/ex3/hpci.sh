
#!/bin/bash

RUN_PATH="/home/vinicius/local/hpctoolkit/bin"

mkdir hpc-i
cd hpc-i
echo "running n -O1"
g++ -O1 -std=c++11 -DN=4000 -DINVERTED ../main.cpp -o main4000io1
$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./main4000io1
$RUN_PATH/hpcstruct ./main4000io1
$RUN_PATH/hpcprof -S main4000io1.hpcstruct -I~ hpctoolkit-main4000io1-measurements

echo "running n -O2"
g++ -O2 -std=c++11 -DN=4000 -DINVERTED ../main.cpp -o main4000io2
$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./main4000io2
$RUN_PATH/hpcstruct ./main4000io2
$RUN_PATH/hpcprof -S main4000io2.hpcstruct -I~ hpctoolkit-main4000io2-measurements

echo "running n -O3"
g++ -O3 -std=c++11 -DN=4000 -DINVERTED ../main.cpp -o main4000io3
$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./main4000io3
$RUN_PATH/hpcstruct ./main4000io3
$RUN_PATH/hpcprof -S main4000io3.hpcstruct -I~ hpctoolkit-main4000io3-measurements



#!/bin/bash

RUN_PATH="/home/vinicius/local/hpctoolkit/bin"

echo "running n"
$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./main 4000 n
$RUN_PATH/hpcstruct ./main 4000 n
$RUN_PATH/hpcprof -S main4000n.hpcstruct -I~ hpctoolkit-main4000n-measurements

echo "running i"
$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./main 4000 i
$RUN_PATH/hpcstruct ./main 4000 i
$RUN_PATH/hpcprof -S main4000i.hpcstruct -I~ hpctoolkit-main4000i-measurements

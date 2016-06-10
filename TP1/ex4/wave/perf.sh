
#!/bin/bash

RUN_PATH="/home/vinicius/local/hpctoolkit/bin"

$RUN_PATH/hpcrun -e PAPI_L2_TCM -e PAPI_L1_TCM -e CPUTIME ./wave.exe
$RUN_PATH/hpcstruct ./wave.exe
$RUN_PATH/hpcprof -S wave.exe.hpcstruct -I* hpctoolkit-wave.exe-measurements

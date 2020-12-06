#!/bin/bash
bc=`echo -n $1 | rev | cut -d'.' -f 2- | rev`".bc"
clang -fno-discard-value-names -O0 -c -emit-llvm $1 -o $bc
opt -instnamer -time-passes -load ./main.so -Sign_Analysis < $bc 1>/dev/null
opt -dot-callgraph $bc
dot -Tpng $bc.callgraph.dot -o CFG.png
#for linux : xdg-open CFG.png
#windows : start CFG.png
xdg-open CFG.png

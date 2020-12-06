#!/bin/bash
bc=`echo -n $1 | rev | cut -d'.' -f 2- | rev`".bc"
clang -fno-discard-value-names -O0 -c -emit-llvm $1 -o $bc
opt -instnamer -time-passes -load ./mainc.so -const_analysis < $bc 1>/dev/null

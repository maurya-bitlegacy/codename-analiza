#!/bin/bash
bc=`echo -n $1 | rev | cut -d'.' -f 2- | rev`".bc"
clang -c -emit-llvm $1 -o $bc
make
opt -instnamer -load ./main.dylib -sign_analysis < $bc 1>/dev/null
opt -dot-callgraph $bc
dot -Tpng tests/sign_test1.bc.callgraph.dot -o e.png
#for linux : xdg-open e.png
#windows : start e.png
open e.png

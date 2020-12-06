#ifndef _CONST_H_
#define _CONST_H_

#include <string>
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/StringRef.h"

#define RETURN "return"
#define BRANCH "branch"

enum PossibleValueKinds {
  UNDEFINED,
  CONSTANT,
  NAC
};

typedef llvm::DenseMap<llvm::StringRef, PossibleValueKinds> Constantv;

std::string CONST_toString(PossibleValueKinds s);

#endif

#ifndef _SIGN_H_
#define _SIGN_H_ // If _SIGN_H_ is not defined, define _SIGN_H_

#include <string>

#include "llvm/ADT/DenseMap.h" // https://stackoverflow.com/questions/43191216/differences-similarities-between-llvmdensemap-and-stdmap
#include "llvm/ADT/StringRef.h" // Constant String Reference Wrapper https://opensource.apple.com/source/lldb/lldb-76/llvm/include/llvm/ADT/StringRef.h.auto.html

#define RETURN "return"
#define BRANCH "branch"

enum SIGN {
  TOP,
  BOTTOM,
  POSITIVE,
  NEGATIVE,
  ZERO,
  INVALID
};

typedef llvm::DenseMap<llvm::StringRef, SIGN> Sign;

SIGN SIGN_meet(SIGN l, SIGN r);

SIGN SIGN_plus(SIGN l, SIGN r);

SIGN SIGN_negate(SIGN s);

SIGN SIGN_mult(SIGN l, SIGN r);


std::string SIGN_toString(SIGN s);

#endif

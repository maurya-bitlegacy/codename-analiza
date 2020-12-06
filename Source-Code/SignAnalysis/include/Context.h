#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "SignAnalysis.Sign.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "llvm/Pass.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/IR/Use.h"
#include "llvm/IR/User.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/Local.h"


template<class M, class N, class A>

class Context {
  private:
    static int count;
    int id;
    M method;
    std::vector<N> worklist;
    std::unordered_map<N, A> out_values;
    std::unordered_map<N, A> in_values;
    A entry_value;
    A exit_value;
    bool analysed;
    bool reverse;

  public:
    bool is_null;
    Context();
    Context(M method, bool reverse);
    bool operator==(const Context& c) const;

    int getId(void) const;
    M getMethod(void);
    std::vector<N> getControlFlowGraph(void);
    std::vector<N> getPredsOf(N _node);
    std::vector<N> getSuccsOf(N _node);
    std::vector<N> getTails(void);
    N getEntryNode(void);
    A getEntryValue(void);
    A getExitValue(void);
    A getValueBefore(N _node);
    A getValueAfter(N _node);

    bool isAnalysed(void);
    void markAnalysed(void);
    void unmarkAnalysed(void);
    void setEntryValue(A _entry_value);
    void setExitValue(A _exit_value);
    void setValueBefore(N _node, A _value);
    void setValueAfter(N _node, A _value);
    bool isCall(N _node);
    void addToWorklist(N _node);
    bool isEmptyWorklist(void);
    N getAndPopWorklist(void);
    N getLastWorklist(void);
    std::vector<N> getWorklist(void);

};

namespace std {
	template<class M, class N, class A>
	struct hash<Context<M, N, A>> {
		size_t operator()(const Context<M, N, A>& c) const {
			return hash<int>()(c.getId());
		}
	};
}

#endif

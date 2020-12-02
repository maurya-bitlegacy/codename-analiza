
#include "forward_inter_procedural_analysis.h"
// #include "DataflowAnalysis.h"

#include "llvm/ADT/APSInt.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include <bitset>
#include <memory>
#include <string>

#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
#include <llvm/ADT/DenseMap.h>
#include "llvm/ADT/DenseSet.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Use.h"
#include "llvm/Transforms/Utils/Local.h"
#include <iterator>
#include <utility>
#include <vector>
#include "constant-analysis.h"
#define RETURN "return"
#define BRANCH "branch"

// enum class PossibleValueKinds : char {
//   UNDEFINED,
//   CONSTANT,
//   NAC
// };

// enum  PossibleValueKinds {
//   UNDEFINED,
//   CONSTANT,
//   NAC
// };

// using std::string;
// typedef llvm::DenseMap<llvm::StringRef, PossibleValueKinds> Constantv;


// struct Constantv
// {
//   PossibleValueKinds kind;
//   llvm::Constant* value;

//   Constantv()
//     : kind{PossibleValueKinds::UNDEFINED},
//       value{nullptr}
//       { }

//   explicit Constantv(llvm::Constant* value)
//     : kind{PossibleValueKinds::CONSTANT},
//       value{value}
//       { }

//   explicit Constantv(PossibleValueKinds kind)
//     : kind{kind},
//       value{nullptr} {
//     assert(kind != PossibleValueKinds::CONSTANT && "Constant kind without a value in ConstantValue constructor.");
//   }

//   Constantv operator|(const Constantv& other) 
//   {
//     if (PossibleValueKinds::UNDEFINED == kind) {
//       return other;
//     } else if ((PossibleValueKinds::UNDEFINED == other.kind) || *this == other) {
//       return *this;
//     } else {
//       return Constantv{PossibleValueKinds::NAC};
//     }
//   }

//   bool operator==(const Constantv& other) const
//   {
//     return kind == other.kind && value == other.value;
//   }

//   bool isConstant()
//   {
//     return kind == PossibleValueKinds::CONSTANT;
//   }

//   bool isVaried()
//   {
//     return kind == PossibleValueKinds::NAC;
//   }
// };

// using ConstantState  = analysis::AbstractState<Constantv>;

using namespace llvm;
   std::string CONSTANT_toString(PossibleValueKinds s)
	{
		errs()<<"CONSTANT.cpp CONSTANT_toString\n";
	  if(s == PossibleValueKinds::NAC) {
	    return "_|_";
	  } else if(s == PossibleValueKinds::UNDEFINED) {
	    return "T";
	  } else if(s == PossibleValueKinds::CONSTANT) {
	    return "0";
	  }else {
	    return "INVALID";
	  }
	}

	  bool sort_func(StringRef a, StringRef b) {
    return std::stoi("0" + a.substr(3).lower()) < std::stoi("0" + b.substr(3).lower());
  }

	//meet function
	// Constantv ConstantAnalysis:: meet(Constantv s1, Constantv s2)
	// {
 //    	return s1 | s2;
 // 	}

   Constantv ConstantAnalysis::normalFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Constantv>> context, BasicBlock* node, Constantv in_value)
  {
    Constantv out_value(in_value);

    for(auto &I: *node)
    {
      Instruction* Insn = &I;
      // std::pair<StringRef, CONSTANT> p = assign(Insn, out_value);
      // if(p.first != BRANCH)
      //   out_value[p.first] = p.second;
    }
    return out_value;
  }

   Constantv ConstantAnalysis::callEntryFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Constantv>> context, Function* target_method, Instruction* Insn, Constantv in_value)
   {
    Constantv entryValue=topValue();
    
    CallInst* call = dyn_cast<CallInst>(Insn);
    unsigned ii = call->getNumArgOperands();
    Constantv entry_value;

    for(unsigned i=0;i<ii;i++)
    {
      Value* op = call->getArgOperand(i);
      Function::arg_iterator AI = target_method->arg_begin();
      if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
        if(CI->isZero()) {
          entry_value[AI[i].getName()] = PossibleValueKinds::CONSTANT;
        } else {
          entry_value[AI[i].getName()] = PossibleValueKinds::CONSTANT;
        } 
      } else {
        if(in_value.find(op->getName()) != in_value.end()) {
          entry_value[AI[i].getName()] = in_value[op->getName()];
        } else {
          assert(false);
        }
      }
    }
    return entry_value;
  }

 	Constantv ConstantAnalysis::callExitFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Constantv>> context, Function* target_method, Instruction* Insn, Constantv exit_value)
 	{
    	Constantv return_value;
    	Value* LHS = dyn_cast<Value>(Insn);
    	return_value[LHS->getName()] = exit_value[RETURN];
    	//DBG(errs() << "callExitFlowFunction " << SIGN_toString(exit_value[RETURN]) << "\n";)
    	return return_value;
  	}

	Constantv ConstantAnalysis:: callLocalFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Constantv>> context, BasicBlock* node, Constantv in_value)
	{
		// Initialise result to the input
		Constantv afterCallValue = copy(in_value);

		Value *lhsOp;
		// Remove information for return value (as it's value will flow from the call)
		for(auto &I: *node)
	    {
	    	Instruction* Insn = &I;
	    	//change
			if(isa<StoreInst>(*Insn) || isa<LoadInst>(*Insn))
			{
				// Get operands
				lhsOp = Insn->getOperand(1);
      			// afterCallValue.erase(lhsOp);
      		}
		}
		// Rest of the map remains the same
		return afterCallValue;
	}

	Constantv ConstantAnalysis:: copy(Constantv src)
	{
		return Constantv(src);
	}

	void ConstantAnalysis::printOutput(void)
	{
    for(auto &e: contexts) {
      errs() << "Function Name: " << e.first->getName() << "\n";
      for(int i=0; i<e.second.size(); i++) {
        Constantv entry_value = e.second[i].get().getEntryValue();
        Constantv exit_value = e.second[i].get().getExitValue();

        std::vector<StringRef> entry_keys;
        std::vector<StringRef> exit_keys;

        for(auto &x: entry_value) {
          entry_keys.push_back(x.first);
        }

        std::sort(entry_keys.begin(), entry_keys.end(), sort_func);

        for(auto &x: exit_value) {
          if(entry_value.find(x.first) == entry_value.end() and x.first != RETURN)
            exit_keys.push_back(x.first);
        }

        std::sort(exit_keys.begin(), exit_keys.end(), sort_func);

        errs() << " Context: " << e.second[i].get().getId() << "\n";
        errs() << "  Entry Value:\n";

        for(auto &x: entry_keys) {
          errs() << "   " << x << " " << CONSTANT_toString(entry_value[x]) << "\n";
        }

        errs() << "  Intermediate Values:\n";
        for(auto &x: exit_keys) {
          errs() << "   " << x << " " << CONSTANT_toString(exit_value[x]) << "\n";
        }

        errs() << "  Return Value:\n";
        if(exit_value.find(RETURN) != exit_value.end()) {
          errs() << "   " << RETURN << " " << CONSTANT_toString(exit_value[RETURN]) << "\n";
        }
        errs() << "\n";
      }
      errs() << "-------------------------------\n\n";
    }
  }
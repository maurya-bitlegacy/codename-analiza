#include "constant-analysis.h"

namespace llvm {

   std::pair<StringRef, PossibleValueKinds> ConstantAnalysis::assign(Instruction* Insn, Constantv dfv)
   {
    
    Value *LHS, *op, *op1, *op2;
    if(isa<StoreInst>(*Insn)) {
      LHS = Insn->getOperand(1);
      op = Insn->getOperand(0);

      if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
        if(CI->isZero()) {
          return std::make_pair(LHS->getName(), PossibleValueKinds::UNDEFINED);
        } else if(CI->isNegative()) {
          return std::make_pair(LHS->getName(), PossibleValueKinds::CONSTANT);
        } else {
          return std::make_pair(LHS->getName(), PossibleValueKinds::NAC);
        }
      } else {
        if(dfv.find(op->getName()) != dfv.end()) {
          return std::make_pair(LHS->getName(), dfv[op->getName()]);
        } else {
          assert(false);
        }
      }

    } else if(isa<LoadInst>(*Insn)){
      LHS = dyn_cast<Value>(Insn);
      op = Insn->getOperand(0);
      if(ConstantInt *CI = dyn_cast<ConstantInt>(op)){
        if(CI->isZero()) {
          return std::make_pair(LHS->getName(), PossibleValueKinds::UNDEFINED);
        } else if(CI->isNegative()) {
          return std::make_pair(LHS->getName(), PossibleValueKinds::CONSTANT);
        } else {
          return std::make_pair(LHS->getName(), PossibleValueKinds::NAC);
        }
      } else {
        if(dfv.find(op->getName()) != dfv.end()) {
          return std::make_pair(LHS->getName(), dfv[op->getName()]);
        } else {
          assert(false);
        }
      }

    } else if(isa<BinaryOperator>(*Insn)) {
      LHS = dyn_cast<Value>(Insn);
      op1 = Insn->getOperand(0);
      op2 = Insn->getOperand(1);

      PossibleValueKinds l, r;
      if(ConstantInt *CI = dyn_cast<ConstantInt>(op1)){
        if(CI->isZero()) {
          l = PossibleValueKinds::UNDEFINED;
        } else if(CI->isNegative()) {
          l = PossibleValueKinds::CONSTANT;
        } else {
          l = PossibleValueKinds::NAC;
        }
      } else {
        if(dfv.find(op1->getName()) != dfv.end()) {
          l = dfv[op1->getName()];
        } else {
          assert(false);
        }
      }

      if(ConstantInt *CI = dyn_cast<ConstantInt>(op2)){
        if(CI->isZero()) {
          r = PossibleValueKinds::UNDEFINED;
        } else if(CI->isNegative()) {
          r = PossibleValueKinds::CONSTANT;
        } else {
          r = PossibleValueKinds::NAC;
        }
      } else {
        if(dfv.find(op2->getName()) != dfv.end()) {
          r = dfv[op2->getName()];
        } else {
          assert(false);
        }
      }

    } else if(isa<ReturnInst>(*Insn)) {
      LHS = Insn->getOperand(0);
      if(ConstantInt *CI = dyn_cast<ConstantInt>(LHS)){
        if(CI->isZero()) {
          return std::make_pair(RETURN, PossibleValueKinds::UNDEFINED);
        } else if(CI->isNegative()) {
          return std::make_pair(RETURN, PossibleValueKinds::CONSTANT);
        } else {
          return std::make_pair(RETURN, PossibleValueKinds::NAC);
        }
      } else {
        if(dfv.find(LHS->getName()) != dfv.end()) {
          return std::make_pair(RETURN, dfv[LHS->getName()]);
        } else {
          assert(false);
        }
      }
    } else if(isa<BranchInst>(*Insn)) {
      return std::make_pair(BRANCH, PossibleValueKinds::UNDEFINED);
    } else if(isa<AllocaInst>(*Insn)) {
      LHS = dyn_cast<Value>(Insn);
      return std::make_pair(LHS->getName(), PossibleValueKinds::CONSTANT); 
    } else {
      LHS = dyn_cast<Value>(Insn);
      return std::make_pair(LHS->getName(), PossibleValueKinds::NAC);
    }
  }

 Constantv ConstantAnalysis::boundaryValue(Function* entryPoint)
  {
    return Constantv();
  }

  Constantv ConstantAnalysis:: copy(Constantv src)
  {
    return Constantv(src);
  }

  Constantv ConstantAnalysis:: meet(Constantv op1, Constantv op2)
  {
    Constantv result(op1);
    for(auto &x : op2)
    {
     if (op1.find(x.first) != op1.end())
      {
        PossibleValueKinds sign1 = x.second;
        PossibleValueKinds sign2 = op1[x.first];
        if(sign1!=PossibleValueKinds::NAC)
          result[x.first] = PossibleValueKinds::NAC;
      else
      {
        result[x.first] = x.second;
      }
    }
   

  }
   return result;
}

   Function* ConstantAnalysis::getEntryMethod(void)
  {
    return module->getFunction("main");
  }

   Constantv ConstantAnalysis::topValue()
  {
    return Constantv();
  }

   Constantv ConstantAnalysis::normalFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Constantv>> context, BasicBlock* node, Constantv in_value)
  {
    Constantv out_value(in_value);

    for(auto &I: *node)
    {
      Instruction* Insn = &I;
      std::pair<StringRef, PossibleValueKinds> p = assign(Insn, out_value);
      if(p.first != BRANCH)
        out_value[p.first] = p.second;
    }
    return out_value;
  }

   Constantv ConstantAnalysis::callEntryFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Constantv>> context, Function* target_method, Instruction* Insn, Constantv in_value)
   {
    
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
    	return return_value;
  	}

	Constantv ConstantAnalysis:: callLocalFlowFunction(std::reference_wrapper<Context<Function*, BasicBlock*, Constantv>> context, BasicBlock* node, Constantv in_value)
	{
		Constantv afterCallValue = copy(in_value);

		Value *lhsOp;
		for(auto &I: *node)
	    {
	    	Instruction* Insn = &I;
			if(isa<StoreInst>(*Insn) || isa<LoadInst>(*Insn))
			{
				lhsOp = Insn->getOperand(1);
      		}
		}
		return afterCallValue;
	}
    bool sort_func(StringRef a, StringRef b) {
    return std::stoi("0" + a.substr(3).lower()) < std::stoi("0" + b.substr(3).lower());
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
          errs() << "   " << x << " " << CONST_toString(entry_value[x]) << "\n";
        }

        errs() << "  Intermediate Values:\n";
        for(auto &x: exit_keys) {
          errs() << "   " << x << " " << CONST_toString(exit_value[x]) << "\n";
        }

        errs() << "  Return Value:\n";
        if(exit_value.find(RETURN) != exit_value.end()) {
          errs() << "   " << RETURN << " " << CONST_toString(exit_value[RETURN]) << "\n";
        }
        errs() << "\n";
      }
      errs() << "-------------------------------\n\n";
    }
  }
}

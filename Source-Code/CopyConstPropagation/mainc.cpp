#include "constant-analysis.h"

using namespace llvm;

namespace {

  class ConstantAnalysisPass : public ModulePass
  {
    public:
      static char ID;
    	ConstantAnalysisPass() : ModulePass(ID) {}

    	 bool runOnModule(Module &M) override
       {
				ConstantAnalysis S(&M);
				S.doAnalysis();
        S.printOutput();
    		return false;
    	}
  };
}

char ConstantAnalysisPass::ID = 0;

static RegisterPass<ConstantAnalysisPass> Y("const_analysis", "ConstProp Analysis Pass", false, false);

static RegisterStandardPasses Y1(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new ConstantAnalysisPass()); });



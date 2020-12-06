#include "SignAnalysis.h"

using namespace llvm;

namespace {

  class SignAnalysisPass : public ModulePass
  {
    public:
      static char ID;
    	SignAnalysisPass() : ModulePass(ID) {}

    	 bool runOnModule(Module &M) override
       {
				SignAnalysis S(&M);
				S.doAnalysis();
        S.printOutput();
    		return false;
    	}
  };
}

char SignAnalysisPass::ID = 0;

static RegisterPass<SignAnalysisPass> Y("Sign_Analysis", "Sign Analysis", true, true);

static RegisterStandardPasses Y1(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new SignAnalysisPass()); });



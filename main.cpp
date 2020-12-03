#include "sign_analysis.h"

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

static RegisterPass<SignAnalysisPass> Y("sign_analysis", "Sign Analysis Pass", true, true);

static RegisterStandardPasses Y1(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new SignAnalysisPass()); });



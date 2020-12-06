#include "ForwardInterProceduralAnalysis.h"

//create new context
template<class M, class N, class A>
Context<M, N, A> ForwardInterProceduralAnalysis<M, N, A>::initContext(M method, A entryv)
{
  Context<M, N, A> *context = new Context<M, N, A>(method, false);

  //create CFG for a context
  std::vector<N> cfg = context->getControlFlowGraph();

  //for every BB in CFG
  for(int i=0; i<cfg.size(); i++)
  {
    context->setValueBefore(cfg[i], topValue()); // creates "in"
    context->setValueAfter(cfg[i], topValue()); // creates "out"
  }

  //add last node to wL
  context->addToWorklist(cfg[cfg.size() - 1]);

  //copy sign of entry value and set it as entry value for that context
  context->setEntryValue(copy(entryv));
  //copy sign of entry value to IN of entry BB of that context
  context->setValueBefore(context->getEntryNode(), copy(entryv));
  //set exit value of context 
  context->setExitValue(topValue());

  contexts[method].push_back(std::ref(*context));

  workList.push_back(std::ref(*context));

  return *context;
}

template<class M, class N, class A>
void ForwardInterProceduralAnalysis<M, N, A>::doAnalysis(void)
{
  //get the entry method from symbol table: i.e. main
   M entry = getEntryMethod();
	 initContext(entry, boundaryValue(entry));

	while(!workList.empty())
  {  
		std::reference_wrapper<Context<M, N, A>> current_context = workList.back();

		if(current_context.get().isEmptyWorklist())
    {
      current_context.get().markAnalysed(); //mark true
      workList.pop_back();
			continue;
		}

		N node = current_context.get().getAndPopWorklist();

    if(node != NULL)
    {
      llvm::errs() << "working on context: " << current_context.get().getId() << " , parent: "  << node->getParent()->getName() << ":" << node->getName() << "\n";
    }
    else
    {
      //if node is NULL
      llvm::errs() << "working on context: " << current_context.get().getId() << " , parent: " << current_context.get().getMethod()->getName() << ":NULL" << "\n";
    }

		if(node != NULL)
    {
			std::vector<N> predecessors = current_context.get().getPredsOf(node);
			if(predecessors.size() != 0)
      {
        //take meet of IN of all pred
				A in = topValue();
				for(int i=0; i<predecessors.size(); i++)
        {
					A predOut = current_context.get().getValueAfter(predecessors[i]);
					in = meet(in, predOut);
				}
				current_context.get().setValueBefore(node, in);
			}

			A prevOut = current_context.get().getValueAfter(node);
			A in = current_context.get().getValueBefore(node);
      A out;
			if(current_context.get().isCall(node))
      {
        out = callCustomFlowFunction(current_context, node, in);
      }
      else
      {
        out = normalFlowFunction(current_context, node, in);
      }

			out = meet(out, prevOut);

			current_context.get().setValueAfter(node, out);

			if(isEqual(out, prevOut) == false)
      {
        std::vector<N> successors = current_context.get().getSuccsOf(node);
			  for(int i=0; i<successors.size(); i++)
        {
          current_context.get().addToWorklist(successors[i]);
				}
			}

      std::vector<N> tails = current_context.get().getTails();
      
      if(current_context.get().isEmptyWorklist())
      {
  			if(std::find(tails.begin(), tails.end(), node) != tails.end())
        {
          current_context.get().addToWorklist(NULL);
  			}
      }

    }
    else
    {
			assert(current_context.get().isEmptyWorklist());

			A exit_value = topValue();
      std::vector<N> tails = current_context.get().getTails();
			for(int i=0; i<tails.size(); i++)
      {  
				A tail_out = current_context.get().getValueAfter(tails[i]);
				exit_value = meet(exit_value, tail_out);
			}

      A prev_exit_value = current_context.get().getExitValue();
			current_context.get().setExitValue(exit_value);
			current_context.get().markAnalysed();
			std::vector<CallSite<M, N, A>> callers =  context_transitions.getCallers(current_context.get());
			if(!callers.empty() and !isReturnEqual(prev_exit_value, exit_value))
      {
        for(auto &call_site: callers)
        {
					std::reference_wrapper<Context<M, N, A>> calling_context = getContextbyId(call_site.second->getParent(), call_site.first.getId());
					N call_node = call_site.second;
          if(calling_context.get().getLastWorklist() == NULL)
            calling_context.get().addToWorklist(call_node);
					workList.push_back(calling_context);
				}
			}
		}
  }
}

template class ForwardInterProceduralAnalysis<llvm::Function*, llvm::BasicBlock*, Sign>;

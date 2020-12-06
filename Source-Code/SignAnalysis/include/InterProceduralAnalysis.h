#ifndef _INTER_PROCEDURAL_ANALYSIS_H_
#define _INTER_PROCEDURAL_ANALYSIS_H_

#include "ContextTransitionTable.h"

#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <functional>
#include <iostream>

template<class M,class N,class A>
class InterProceduralAnalysis {
  protected:
    std::vector<std::reference_wrapper<Context<M,N,A>>> workList;
    std::unordered_map<M, std::vector<std::reference_wrapper<Context<M,N,A>>>> contexts;
    ContextTransitionTable<M,N,A> context_transitions;
    bool reverse;

  public:
    InterProceduralAnalysis(bool reverse): reverse(reverse) { }

    virtual A topValue() = 0;
    virtual A copy(A src) = 0;
    virtual A boundaryValue(M entryPoint) = 0;
    virtual void doAnalysis(void) = 0;
    virtual A meet(A op1, A op2) = 0;
    virtual bool isEqual(A op1, A op2) = 0;
    virtual bool isReturnEqual(A op1, A op2) = 0;

    std::vector<CallSite<M,N,A>> getCallers(Context<M,N,A> target);
    std::reference_wrapper<Context<M,N,A>> getContext(M method, A value);
    std::reference_wrapper<Context<M,N,A>> getContextbyId(M method, int id);
    std::vector<std::reference_wrapper<Context<M,N,A>>> getContexts(M method);
    ContextTransitionTable<M,N,A> getContextTransitionTable();
    std::unordered_set<M> getMethods();
    std::unordered_map<M, Context<M,N,A>> getTargets(CallSite<M,N,A> call_site);
};
#endif

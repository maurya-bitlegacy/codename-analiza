#include "context.h"


template<class M, class N, class A>
Context<M, N, A>::Context(M method, bool reverse): method(method), analysed(false), is_null(false), reverse(reverse) {
	id = ++count;
}

template<class M, class N, class A>
bool Context<M, N, A>::operator==(const Context<M, N, A>& c) const {
	return id == c.getId();
}
template<class M, class N, class A>
Context<M, N, A>::Context(): analysed(false), is_null(true) {
}

template<class M, class N, class A>
int Context<M, N, A>::getId(void) const
{
	return id;
}

template<class M, class N, class A>
M Context<M, N, A>::getMethod(void)
{
	return method;
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getControlFlowGraph(void)
{
	std::vector<N> wl;
	for(N BB: depth_first(&method->getEntryBlock())) 
	{
  		wl.push_back(BB);
  	}
	std::reverse(wl.begin(), wl.end());
	return wl;
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getPredsOf(N no)
{
	std::vector<N> wl;
	for(N P: predecessors(no))
	{
		wl.push_back(P);
	}

	return wl;
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getSuccsOf(N no)
{
	std::vector<N> wl;
	for(N S: successors(no))
	{
		wl.push_back(S);
	}

	return wl;
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getTails(void) {
	std::vector<N> wl;
	for(N BB: depth_first(&method->getEntryBlock())) {
		for(auto &I: *BB) {
			if(llvm::isa<llvm::ReturnInst>(I)) {
				wl.push_back(BB);
				break;
			}
		}
	}
	return wl;
}

template<class M, class N, class A>
bool Context<M, N, A>::isCall(N _node) {
	for(auto &I: *_node) {
		if(llvm::isa<llvm::CallInst>(I)) {
			return true;
		}
	}
	return false;
}

template<class M, class N, class A>
N Context<M, N, A>::getEntryNode(void)
{
	return &method->getEntryBlock();
}

template<class M, class N, class A>
A Context<M, N, A>::getEntryValue(void)
{
	return entry_value;
}

template<class M, class N, class A>
A Context<M, N, A>::getExitValue(void) {
	return exit_value;
}

template<class M, class N, class A>
A Context<M, N, A>::getValueBefore(N no)
{
	return in_values[no];
}

template<class M, class N, class A>
A Context<M, N, A>::getValueAfter(N no)
{
	return out_values[no];
}

template<class M, class N, class A>
bool Context<M, N, A>::isAnalysed(void)
{
	return analysed;
}

template<class M, class N, class A>
void Context<M, N, A>::markAnalysed(void)
{
	analysed = true;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::unmarkAnalysed(void)
{
	analysed = false;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::setEntryValue(A entryv)
{
	entry_value = entryv;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::setExitValue(A exitv)
{
	exit_value = exitv;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::setValueBefore(N no, A val)
{
	in_values[no] = val;
	return;
}

template<class M, class N, class A>
void Context<M, N, A>::setValueAfter(N no, A val)
{
	out_values[no] = val;
	return;
}


template<class M, class N, class A>
void Context<M, N, A>::addToWorklist(N no)
{
	worklist.push_back(no);
	return;
}

template<class M, class N, class A>
bool Context<M, N, A>::isEmptyWorklist(void)
{
	return worklist.empty();
}

template<class M, class N, class A>
std::vector<N> Context<M, N, A>::getWorklist(void) {
	return worklist;
}

template<class M, class N, class A>
N Context<M, N, A>::getAndPopWorklist(void)
{
	N no = worklist.back();
	worklist.pop_back();
	return no;
}

template<class M, class N, class A>
N Context<M, N, A>::getLastWorklist(void) {
	if(worklist.empty()) return NULL;
	return worklist.back();
}


template<class M, class N, class A>
int Context<M, N, A>::count = 0;

template class Context<llvm::Function*, llvm::BasicBlock*, Constantv>;

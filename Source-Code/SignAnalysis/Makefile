LLVM_CONFIG = llvm-config
OBJECTS = main.o Context.o ContextTransitionTable.o ForwardInterProceduralAnalysis.o SignAnalysis.Sign.o InterProceduralAnalysis.o SignAnalysis.o
CXX = clang++

CXXFLAGS = -c -Iinclude/ `$(LLVM_CONFIG) --cxxflags --system-libs` -g -Wall -fPIC -fno-rtti
LDFLAGS = `$(LLVM_CONFIG) --ldflags` -Wl,-dead_strip -Wl,-flat_namespace -Wl,-undefined -Wl,suppress

all: main.so

main.so: $(OBJECTS)
	$(CXX) -shared -dylib $^ -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o *.so tests/*.bc

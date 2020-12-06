LLVM_CONFIG = llvm-config
OBJECTS = mainc.o context.o contextTransitionTable.o forwardInterProceduralAnalysis.o const.o constant-analysis.o interProceduralAnalysis.o
CXX = clang++

CXXFLAGS = -c -I include/ `$(LLVM_CONFIG) --cxxflags --system-libs` -g -Wall -fno-rtti
LDFLAGS = `$(LLVM_CONFIG) --ldflags` -Wl,-dead_strip -Wl,-flat_namespace -Wl,-undefined -Wl,suppress -lffi
REQUIRES_RTTI=1

all: mainc.so

mainc.so: $(OBJECTS)
	$(CXX) -shared -dylib $^ -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o *.so tests/*.bc

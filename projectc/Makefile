LLVM_CONFIG = llvm-config
OBJECTS = mainc.o context.o context_transition_table.o forward_inter_procedural_analysis.o const.o constant-analysis.o inter_procedural_analysis.o
CXX = clang++

CXXFLAGS = -c -I include/ `$(LLVM_CONFIG) --cxxflags --system-libs` -g -Wall -fno-rtti
LDFLAGS = `$(LLVM_CONFIG) --ldflags` -Wl,-dead_strip -Wl,-flat_namespace -Wl,-undefined -Wl,suppress -lffi
REQUIRES_RTTI=1

all: mainc.dylib

mainc.dylib: $(OBJECTS)
	$(CXX) -shared -dylib $^ -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o *.dylib tests/*.bc

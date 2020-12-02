LLVM_CONFIG = llvm-config
OBJECTS = main.o context.o context_transition_table.o forward_inter_procedural_analysis.o sign_analysis.o inter_procedural_analysis.o sign.o
CXX = clang++

CXXFLAGS = -c -Iinclude/ `$(LLVM_CONFIG) --cxxflags --system-libs` -g -Wall -fPIC -fno-rtti
LDFLAGS = `$(LLVM_CONFIG) --ldflags` -Wl,-dead_strip -Wl,-flat_namespace -Wl,-undefined -Wl,suppress

all: main.dylib

main.dylib: $(OBJECTS)
	$(CXX) -shared -dylib $^ -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o *.dylib tests/*.bc

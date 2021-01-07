bin:
	mkdir bin

defaultheaders = counter.h outputhelpers.h primes.h constants.h makefile

% : %.cpp $(defaultheaders)
	g++ --std=c++17 $< -lmpirxx -lmpir -O3 -o $@

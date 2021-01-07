bin:
	mkdir bin

defaultheaders = counter.h outputhelpers.h primes.h constants.h makefile

bin/sets: bin sets.cpp $(defaultheaders)
	g++ --std=c++17 sets.cpp -lmpirxx -lmpir -O3 -o bin/sets

bin/primebits: bin primebits.cpp $(defaultheaders)
	g++ --std=c++17 primebits.cpp -lmpirxx -lmpir -O3 -o bin/primebits

bin/734: bin 734.cpp $(defaultheaders)
	g++ --std=c++17 734.cpp -lmpirxx -lmpir -O3 -o bin/734
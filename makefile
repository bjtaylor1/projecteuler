bin:
	mkdir bin

defaultheaders = counter.h outputhelpers.h primes.h constants.h makefile

bin/734: bin 734.cpp $(defaultheaders)
	g++ 734.cpp -lmpirxx -lmpir -O3 -o bin/734
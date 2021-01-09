bin:
	mkdir bin

defaultheaders = counter.h outputhelpers.h primes.h constants.h cache.h makefile
libdir=/usr/local/lib

% : %.cpp $(defaultheaders)
	g++ --std=c++17 $< $(libdir)/libmpir.a $(libdir)/libmpirxx.a $(PE_LINK_EXTRA) -O3 -o $@

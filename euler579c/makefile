ALLCPPSRCS=$(wildcard *.cpp)
TESTCPPSRCS=$(wildcard *test.cpp)
CPPSRCS=$(filter-out $(TESTCPPSRCS),$(ALLCPPSRCS))

ALLHSRCS=$(wildcard *.h)
TESTHSRCS=$(wildcard *test.h)
HSRCS=$(filter-out $(TESTHSRCS),$(ALLHSRCS))

OBJS=$(CPPSRCS:.cpp=.o)
ALLOBJS=$(ALLCPPSRCS:.cpp=.o)
TESTOBJS=$(filter-out hdsrv.o,$(ALLOBJS))

LDFLAGS=-g /usr/local/lib/libproj.a -lboost_thread -lboost_system -lboost_filesystem -lboost_regex
TESTLDFLAGS=-pthread $$PUGIXML_ROOT/build/make-g++-debug-standard/src/pugixml.cpp.o 

CXXFLAGS=$(EXTRACXXFLAGS) -O4 
TESTCXXFLAGS=-pthread -I $$GTEST_ROOT -I$$GTEST_ROOT/include -I$$PUGIXML_ROOT
MAINBIN=bin/hdsrv
TESTBIN=bin/tests

all : $(MAINBIN)

test : $(TESTBIN) 
	$(TESTBIN) -d ~/terr50_cgml_gb -p 48879
	
bin : 
	mkdir -p bin

clean :
	$(RM) -f *.gch *.o *.d

sources.d: $(CPPSRCS) $(HSRCS) makefile
	$(CXX) -MM $(CPPSRCS) | sed ':x; /\\$$/ { N; s/\\\n//; tx }' | sed -r -e 's/^(.*)\.o:(.*)$$/\1.o:\2\n\t$$(CXX) $$(CXXFLAGS) -c \1.cpp\n/'>sources.d
#                         ^ combines line continuations                ^ inserts compile command after line 

tests.d: $(TESTCPPSRCS) $(TESTHSRCS) makefile
	$(CXX) -MM $(TESTCPPSRCS) | sed ':x; /\\$$/ { N; s/\\\n//; tx }' | sed -r -e 's/^(.*)\.o:(.*)$$/\1.o:\2\n\t$$(CXX) $$(CXXFLAGS) $$(TESTCXXFLAGS) -c \1.cpp\n/'>tests.d


-include sources.d

-include tests.d

$(MAINBIN) : sources.d $(OBJS) makefile bin
	$(CXX) $(OBJS) $(LDFLAGS) -o $(MAINBIN)

$(TESTBIN) : tests.d $(TESTOBJS) makefile bin
	$(CXX) $(TESTOBJS) $$GTEST_ROOT/src/gtest-all.o $(LDFLAGS) $(TESTLDFLAGS) -o $(TESTBIN)

printvars : 
	@echo CPPSRCS=$(CPPSRCS)
	@echo ALLCPPSRCS=$(ALLCPPSRCS)
	@echo TESTCPPSRCS=$(TESTCPPSRCS)
	@echo TESTOBJS=$(TESTOBJS)

always:

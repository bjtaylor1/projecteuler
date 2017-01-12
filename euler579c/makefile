ALLCPPSRCS=$(wildcard *.cpp)
CPPSRCS=$(filter-out $(TESTCPPSRCS),$(ALLCPPSRCS))

ALLHSRCS=$(wildcard *.h)
HSRCS=$(filter-out $(TESTHSRCS),$(ALLHSRCS))

ALLOBJS=$(ALLCPPSRCS:.cpp=.o)
OBJS=$(CPPSRCS:.cpp=.o)

LDFLAGS=-pthread

CXXFLAGS=$(EXTRACXXFLAGS) -O4 -std=c++11 
MAINBIN=bin/euler579c

all : $(MAINBIN)

bin : 
	mkdir -p bin

clean :
	$(RM) -f *.gch *.o *.d

sources.d: $(CPPSRCS) $(HSRCS) makefile
	$(CXX) -std=c++11 -MM $(CPPSRCS) | sed ':x; /\\$$/ { N; s/\\\n//; tx }' | sed -r -e 's/^(.*)\.o:(.*)$$/\1.o:\2\n\t$$(CXX) $$(CXXFLAGS) -c \1.cpp\n/'>sources.d
#                         ^ combines line continuations                ^ inserts compile command after line 

-include sources.d

$(MAINBIN) : sources.d $(OBJS) makefile bin
	$(CXX) $(OBJS) $(LDFLAGS) -o $(MAINBIN)

printvars : 
	@echo CPPSRCS=$(CPPSRCS)
	@echo ALLCPPSRCS=$(ALLCPPSRCS)
	@echo TESTCPPSRCS=$(TESTCPPSRCS)
	@echo TESTOBJS=$(TESTOBJS)

always:

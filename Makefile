EXEC      = testBdd p2 p5 p6a p6b p6c p7a final
CSRCS     = $(filter-out $(addsuffix .cpp, $(EXEC)), $(wildcard *.cpp))
CHDRS     = $(wildcard *.h)
COBJS     = $(addsuffix .o, $(basename $(CSRCS)))

CXX       = g++
CFLAGS    = -g -Wall -O3 -std=c++17
EXTINCDIR =
LIBDIR    =
ECHO      = /bin/echo


.PHONY: all
all: $(EXEC)

.PHONY: run
run: $(addsuffix -run, $(EXEC))

run-%: %
	./$< | tee $<.out

$(EXEC): %: $(COBJS) %.o
	@$(ECHO) "> building: $@"
	$(CXX) -o $@ $(CFLAGS) $^

%.o : %.cpp
	@$(ECHO) "> compiling: $<"
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(COBJS) $(EXEC)

.PHONY: depend
depend: .depend.mak
.depend.mak: $(CSRCS) $(CHDRS)
	@$(ECHO) Making dependencies ...
	$(CXX) -MM $(DEPENDDIR) $(CSRCS) > $@

include .depend.mak


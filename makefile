# makefile for Vet
# Copyright (C) 2011 by Joshua E Cook

objects = vet
libraries = libvet
tests = vet_vet
tests := $(addprefix tests/,$(tests))

depends := $(addsuffix .mk,$(objects) $(tests))

objects := $(addsuffix .o,$(objects))
libraries := $(addsuffix .a,$(libraries))
tests := $(addsuffix .cpp.out,$(tests)) $(addsuffix .o,$(tests)) $(tests)

CPPFLAGS += -I. -Wall -O0 -g

.PHONY: all check clean
all: $(objects) $(libraries)
check: $(tests)
clean:
	$(RM) $(tests) $(libraries) $(objects) $(depends)

%.a: $(objects)
	$(AR) $(ARFLAGS) '$@' $^

%.mk: CPPFLAGS += -MM -I.
%.mk: %.c
	$(CPP) $(CPPFLAGS) -MF '$@' -MT '$*.o' '$<'

%.cpp.out: %.c
	$(CPP) $(CPPFLAGS) '$<' > '$@'

include $(depends)

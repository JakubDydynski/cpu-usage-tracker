CC = $(TOOLCHAIN)
IDIR = ../include
GCCFLAGS=-Wall -Wextra -g -I$(IDIR) -lpthread
CLANGFLAGS=-Weverything -I$(IDIR) -lpthread -Wno-disabled-macro-expansion -Wno-unused-command-line-argument
LDLIBS=
ODIR = obj
BUILD = build
FLAGS =


_DEPS = main.h util.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o util.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


ifndef TOOLCHAIN
  $(info Undefined toolchain, assigning default one!)
  CC=gcc
  FLAGS = $(GCCFLAGS)
else
	ifeq ($(TOOLCHAIN), clang)
		FLAGS = $(CLANGFLAGS)
	else
		ifeq ($(TOOLCHAIN), gcc)
			FLAGS = $(GCCFLAGS)
		else
			$(error Unsupported toolchain defined!)
		endif
	endif
endif



$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

$(BUILD)/all: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) $(LDLIBS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BUILD)/*

test:

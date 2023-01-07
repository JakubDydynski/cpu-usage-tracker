CC = $(TOOLCHAIN)
IDIR = ../include
GCCFLAGS=-Wall -Wextra -g -I$(IDIR) -lpthread
CLANGFLAGS=-Weverything -I$(IDIR) -lpthread
LDLIBS=
ODIR = obj
FLAGS =

# System powinien wspierać dynamiczne zmiany kompilatora (czytanie zmiennej środowiskowej CC)., czyli coś typu make toolchain= i tutaj gcc lub clang



_DEPS = main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

ifeq ($(TOOLCHAIN), CLANG)
	FLAGS = $(CLANGFLAGS)
else
	FLAGS = $(GCCFLAGS)
endif

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

all: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) $(LDLIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ all

test:
# Aplikacja musi posiadać przynajmniej 1 automatyczny test. Może być to test jednostkowy napisany w
#C (budowany za pomocą make test) lub inny test napisany w C lub w języku skryptowym odpalającym
#cały program i testujący jakieś zachowanie aplikacji
#. Do pisania testów jednostkowych, nie potrzebujesz żadnej dodatkowej biblioteki. Proste testy możesz
#napisać z użyciem wbudowanej funkcji assert()
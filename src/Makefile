CC = gcc
IDIR = ../include
CFLAGS=-Wall -Wextra -g -I$(IDIR) -lpthread
LDLIBS=
ODIR = obj

# System powinien wspierać dynamiczne zmiany kompilatora (czytanie zmiennej środowiskowej CC)., czyli coś typu make toolchain= i tutaj gcc lub clang



_DEPS = main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(iINCDIR)/*~

test:
# Aplikacja musi posiadać przynajmniej 1 automatyczny test. Może być to test jednostkowy napisany w
#C (budowany za pomocą make test) lub inny test napisany w C lub w języku skryptowym odpalającym
#cały program i testujący jakieś zachowanie aplikacji
#. Do pisania testów jednostkowych, nie potrzebujesz żadnej dodatkowej biblioteki. Proste testy możesz
#napisać z użyciem wbudowanej funkcji assert()
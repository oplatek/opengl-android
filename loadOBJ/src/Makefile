# This makefile suppose following structure of dirs: $(IDIR), $(ODIR), $(LDIR)
# e.g. ../include,obj, ../lib/
# The makefile itself shoul be placend in SRC directory!
_DEPS=objload.h
_OBJ=main.o objload.o 
PROG=loaderOBJ
IDIR=../include
CC=g++
CFLAGS=-I$(IDIR) -Wall

ODIR=../obj
LDIR=../lib

LIBS=-lglut -lGLU

DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(PROG)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)




# if we are so stupid and name a file clean, it is now ok
.PHONY: clean

clean:
	rm -f $(PROG) $(ODIR)/*.o *~ core $(IDIR)/*~ 


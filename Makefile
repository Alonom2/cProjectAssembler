CC=gcc
INFILES= macro.c main.c firstPass.c tables.c

CFLAGS= -ansi -Wall -pedantic -g -o

OUTNAME= projectBins

exA: compile run clean

compile:
	$(CC) $(INFILES) $(CFLAGS) $(OUTNAME) 

run:
	./$(OUTNAME) 

clean: 
	rm $(OUTNAME)

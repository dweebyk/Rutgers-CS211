CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

all: formula

formula: formula.c nCr.o
	$(CC) formula.c nCr.o $(CFLAGS) $(LIBS) -o formula

nCr.o: nCr.s
	$(CC) -o nCr.o $(CFLAGS) -c nCr.s

clean:
	rm formula
	rm nCr.o
	@echo 'Thanks for grading! :)'

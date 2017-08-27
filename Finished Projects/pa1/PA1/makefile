CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

all: calc format
	@echo 'Multiplication is implemented but decimal values tend to overflow very quickly.'


calc: 
	$(CC) calc.c helper.c $(CFLAGS) $(LIBS) -o calc

format:
	$(CC) format.c helper.c $(CFLAGS) $(LIBS) -o format

clean:
	rm calc
	rm format
	@echo 'Thanks for grading! :)'

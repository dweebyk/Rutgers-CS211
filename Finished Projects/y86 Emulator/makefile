CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

all: y86emul y86dis

y86emul: y86emul.c helper.c 
		$(CC) y86emul.c helper.c $(CFLAGS) $(LIBS) -o y86emul

y86dis: y86dis.c helper.c
		$(CC) y86dis.c helper.c $(CFLAGS) $(LIBS) -o y86dis

clean:
		rm y86emul
		rm y86dis
		@echo 'Thanks for grading! :)'

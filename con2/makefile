CC=gcc
CFLAGS=-Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -g

all: con2

con2: con2.o
	$(CC) con2.o -o a1 -pthread

con2.o:
	$(CC) -c con2.c $(CFLAGS)

clean:
	rm -rf *.o a1 *~ *.aux *.out
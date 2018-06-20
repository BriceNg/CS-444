CC=gcc
CFLAGS=-Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -g

all: conc1

conc1: conc1.o
	$(CC) conc1.o -o a1 -pthread

conc1.o:
	$(CC) -c conc1.c $(CFLAGS)

clean:
	rm -rf *.o a1 *~ *.aux *.out
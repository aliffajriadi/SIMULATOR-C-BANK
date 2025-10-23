CC = gcc
CFLAGS = -Wall -std=c11
SRCDIR = src
OBJS = main.o bank.o

all: bank

bank: $(OBJS)
	$(CC) $(CFLAGS) -o bank $(OBJS)

main.o: $(SRCDIR)/main.c $(SRCDIR)/bank.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o main.o

bank.o: $(SRCDIR)/bank.c $(SRCDIR)/bank.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/bank.c -o bank.o

clean:
	rm -f *.o bank

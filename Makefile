CC = gcc
CFLAGS = -Wall -std=c11
OBJS = main.o bank.o

all: bank

bank: $(OBJS)
	$(CC) $(CFLAGS) -o bank $(OBJS)

main.o: main.c bank.h
	$(CC) $(CFLAGS) -c main.c

bank.o: bank.c bank.h
	$(CC) $(CFLAGS) -c bank.c

clean:
	rm -f *.o bank

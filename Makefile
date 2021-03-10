CC= gcc
#CFLAGS= -Wall -w -O3 -g -std=c99 -mtune=native -DNDEBUG
#CFLAGS= -Wall -w -O3 -g -std=c99 -march=native -DNDEBUG
CFLAGS= -Werror -std=c11

all:
	$(CC) $(CFLAGS) fixedhash.c -o fixedhash.out
	$(CC) $(CFLAGS) fixedmem.c -o fixedmem.out
	$(CC) $(CFLAGS) fixedll.c -o fixedll.out
	$(CC) $(CFLAGS) dyntree.c -o dyntree.out
	$(CC) $(CFLAGS) bigstack.c -o bigstack.out

clean:
	rm -f *.exe *.out *.o

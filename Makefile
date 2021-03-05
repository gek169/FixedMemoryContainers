CC= gcc
#CFLAGS= -Wall -w -O3 -g -std=c99 -mtune=native -DNDEBUG
#CFLAGS= -Wall -w -O3 -g -std=c99 -march=native -DNDEBUG
CFLAGS= -Werror -std=c99

all:
	$(CC) $(CFLAGS) fixedhash.c -o fixedhash.out
	$(CC) $(CFLAGS) fixedmem.c -o fixedmem.out
	$(CC) $(CFLAGS) fixedll.c -o fixedll.out

clean:
	rm -f *.exe *.out *.o

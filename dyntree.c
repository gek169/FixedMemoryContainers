

#include "dynmemtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned int uint;
void uint_cleanup_dummy(uint* a){(void)a;};
void uint_constructor_dummy(uint* a){*a = 0;};
//define types.
DYNTREE(char*, mybin, 4);
//Table type
TABLE(uint, mytable, 3, uint_constructor_dummy, uint_cleanup_dummy);
//Block- array.
BLOCK(uint, myblock, 3);
//Dynblock- a vector.
DYNBLOCK(unsigned int, mydynblock);


mybin b = (mybin){0, {0}}; //When declared global, it is already in its initialized state.
//Demonstrate ordinary dyntree
void create_c(mybin* t, uint8_t iter){
	mybin* d1 = calloc(1, sizeof(mybin));
	d1->d = calloc(1,10);
	mybin* d2 = calloc(1, sizeof(mybin));
	d2->d = calloc(1,10);
	t->c[0] = d1;
	t->c[1] = d2;
	if(iter > 1){
		create_c(d1, iter-1);
		create_c(d2, iter-1);
	}
}
int main(){
	/*
	b.d = calloc(1,200);
	create_c(&b, 20);
	mybin_cleanup(&b);
	*/
	mytable q;
	mytable_init(&q);
	*mytable_lazy_get(&q, 3) = 5;
	*mytable_lazy_get(&q, 1) = 7;
	*mytable_lazy_get(&q, 0) = 9;
	for(unsigned i = 0; i < mytable_size; i++)
		if(mytable_get(&q, i))
			printf("value at %u is is %u\n", i, *mytable_lazy_get(&q, i));
	mytable_cleanup(&q);
	puts("\n\n");
	myblock bruh;
	myblock_init(&bruh);
	*myblock_get(&bruh, 901237) = 5;
	*myblock_get(&bruh, 375) = 9;
	for(unsigned i = 0; i < myblock_size; i++)
		printf("value at %u is is %u\n", i, *myblock_get(&bruh, i));
	myblock_cleanup(&bruh);
	
	puts("\n\n");
	mydynblock bruh2;
	mydynblock_init(&bruh2, 3);
	*mydynblock_get(&bruh2, 901237) = 5;
	*mydynblock_get(&bruh2, 375) = 9;
	mydynblock_resize(&bruh2, 4);
		for(unsigned i = 0; i < mydynblock_getsize(&bruh2); i++)
		printf("value at %u is is %u\n", i, *mydynblock_get(&bruh2, i));
	mydynblock_cleanup(&bruh2);
}

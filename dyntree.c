

#include "dyn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned int uint;
static inline void uint_cleanup_dummy(uint* a){printf("Cleaning up %u\n", *a);}
static inline void charp_cleanup_dummy(char** a){(void)a;}
static inline void uint_constructor_dummy(uint* a){*a = 0;}
static inline void uint_flood_dummy(uint* a){printf("Being asked to flood a %u\n", *a);}
//define types.
DYNTREE(char*, mybin, 4, charp_cleanup_dummy);
//Table type
//TABLE(uint, mytable, 3, uint_constructor_dummy, uint_cleanup_dummy, uint_constructor_dummy);
//The last argument is the "flood" argument,
//the constructor to invoke on flood.
TABLE(uint, mytable, 3, uint_constructor_dummy, uint_cleanup_dummy, uint_flood_dummy);
/*Declare a second level of table, eight of mytable*/
TABLE(mytable, table2, 4, mytable_init, mytable_cleanup, mytable_flood);
//Block- array.
BLOCK(uint, myblock, 3, uint_constructor_dummy, uint_cleanup_dummy);


//define types uint_ptr, uint_borrow, 
//and functions uint_malloc, uint_calloc, and borrow_uint, which returns a uint_borrow.
PTR(uint)
//Dynblock- a vector.
//Do not use it, the type system cannot guarantee its size at compile time.
//DYNBLOCK(uint, mydynblock, uint_constructor_dummy, uint_cleanup_dummy);


mybin b = {0}; //When declared global, it is already in its initialized state.
//Demonstrate ordinary dyntree
void create_c(mybin* t, uint8_t iter){
	mybin* d1 = calloc(1, sizeof(mybin));
	d1->d = calloc(1,100);
	mybin* d2 = calloc(1, sizeof(mybin));
	d2->d = calloc(1,100);
	t->c[0] = d1;
	t->c[1] = d2;
	if(iter > 1){
		create_c(d1, iter-1);
		create_c(d2, iter-1);
	}
}

void dummy(void* a){
	printf("Asked to destroy a %zu!\n", (size_t)a);
}

typedef struct{
	void* a;
	void* b;
	int x;
	int y;
} mycustomstruct;

CREATE_DESTRUCTOR(destroy_mycustomstruct, mycustomstruct, 2, 
structure->a, dummy, 
structure->b, dummy);

int main(){
	b.d = calloc(1,6);
	create_c(&b, 5);
	mybin_cleanup(&b);
{
	mycustomstruct bun = {0};
	bun.a = malloc(10);
	bun.b = malloc(20);
	destroy_mycustomstruct(&bun);
}	
	mytable q = {0};
	mytable_init(&q);
	*mytable_lazy_get(&q, 3) = 5;
	*mytable_lazy_get(&q, 1) = 7;
	*mytable_lazy_get(&q, 0) = 9;
	for(unsigned i = 0; i < mytable_size; i++)
		if(mytable_get(&q, i))
			printf("value at %u is is %u\n", i, *mytable_lazy_get(&q, i));
	mytable_flood(&q); //flood the table- which means "occupy all memory"
	puts("\nFLOODING TABLE...\n");
	for(unsigned i = 0; i < mytable_size; i++)
			if(mytable_get(&q, i))
				printf("value at %u is is %u\n", i, *mytable_lazy_get(&q, i));
	mytable_cleanup(&q);
	puts("\n~~TESTING MULTI-LEVEL TABLE~~\n");
	table2 supertable = {0};
	table2_init(&supertable);
	*mytable_lazy_get(
	table2_lazy_get(&supertable, 3),0) = 5;
	*mytable_lazy_get(
	table2_lazy_get(&supertable, 3),1) = 7;
	/*Indexing out of bounds! Oh no!*/
	*mytable_lazy_get(
	table2_lazy_get(&supertable, 490816),48907) = 974;
	for(unsigned i = 0; i < table2_size; i++){
		mytable* lq = table2_get(&supertable, i);
		if(lq){ /*Memory exists...*/
			for(unsigned j = 0; j < mytable_size; j++)
				if(mytable_get(lq, j))
					printf("value at %u,%u is is %u\n", i,j, *mytable_get(lq, j));
		}
	}
	puts("\n~~~FLOODING THE MULTITABLE~~~\n");
	table2_flood(&supertable);
	for(unsigned i = 0; i < table2_size; i++){
			mytable* lq = table2_get(&supertable, i);
			if(lq){ /*Memory exists...*/
				for(unsigned j = 0; j < mytable_size; j++)
					if(mytable_get(lq, j))
						printf("value at %u,%u is is %u\n", i,j, *mytable_get(lq, j));
			}
		}
	
	table2_cleanup(&supertable);
	puts("\n\n");
	myblock bruh = {0};
	myblock_init(&bruh);
	*myblock_get(&bruh, 901237) = 5;
	*myblock_get(&bruh, 375) = 9;
	for(unsigned i = 0; i < myblock_size; i++)
		printf("value at %u is is %u\n", i, *myblock_get(&bruh, i));
	myblock_cleanup(&bruh);
	
	puts("\n\n");
	/*
	mydynblock bruh2;
	mydynblock_init(&bruh2, 3);
	*mydynblock_get(&bruh2, 901237) = 5;
	*mydynblock_get(&bruh2, 375) = 9;
	mydynblock_resize(&bruh2, 5);
	mydynblock_resize(&bruh2, 3);
		for(unsigned i = 0; i < mydynblock_getsize(&bruh2); i++)
		printf("value at %u is is %u\n", i, *mydynblock_get(&bruh2, i));
	mydynblock_cleanup(&bruh2);
	*/
	//Proof of concept- safe loops
	//Also, safe pointer demonstration. Forces you to track size.
	uint_ptr my_uint_array = {0};
	my_uint_array = uint_calloc(27);
	#define LOOP(v, e)\
	for(unsigned long long v = 0, __internal_##v = 0,__internal_test_##v[2*(e>0)-1]; __internal_##v < e; __internal_##v++, v = __internal_##v)
	LOOP(i,29)
	{
		if(my_uint_array.p && my_uint_array.size > i)
			printf("value at %u is %u\n", (unsigned int)i, my_uint_array.p[i]);
	}
	uint_free(my_uint_array);
	void* multi[10];
	for(int i = 0; i < 10; i++)
		multi[i] = malloc(100); //100 bytes
	multifree(10, multi[0], dummy
				, multi[1], dummy	
				, multi[2], dummy
				, multi[3], dummy
				, multi[4], dummy
				, multi[5], dummy
				, multi[6], dummy
				, multi[7], dummy
				, multi[8], dummy
				, multi[9], dummy
				);
}

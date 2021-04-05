#include <stdio.h>
#include <stdlib.h>


#include "dynmemtree.h"

DYNTREE(char*, mybin, 4);

mybin b = (mybin){0, {0}}; //When declared global, it is already in its initialized state.

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
	b.d = calloc(1,200);
	create_c(&b, 20);
	mybin_cleanup(&b);
}

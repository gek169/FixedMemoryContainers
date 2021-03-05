#include <stdio.h>
#include <stdlib.h>


#include "dynmemtree.h"

DYNTREE(void, mybin, 2);

mybin b; //When declared global, it is already in its initialized state.

void create_children(mybin* t, uint8_t iter){
	mybin* d1 = calloc(1, sizeof(mybin));
	d1->data = malloc(10);
	mybin* d2 = calloc(1, sizeof(mybin));
	d2->data = malloc(10);
	t->children[0] = d1;
	t->children[1] = d2;
	if(iter > 1){
		create_children(d1, iter-1);
		create_children(d2, iter-1);
	}
}
int main(){
	b.data = malloc(200);
	create_children(&b, 13);
	mybin_cleanup(&b);
	//End of program.
}

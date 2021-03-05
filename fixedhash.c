#include "fixedmem.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct {
	unsigned int id; //Required by hashmap. note that id = 0 is an invalid hash value, it registers as "unused."
	unsigned int data;
} hdata;

//typename, name of the hashmap, power of 2 of the width, depth.
//NOTE: 0 is an invalid id
FIXEDMEM_HASHMAP(hdata, myhmap, 8, 4);

int main(){
	for(unsigned int i = 1; i < 33120; i+= 74){
		hdata mine;
		mine.id = i;
		mine.data = rand();
		hdata* targ = myhmap_getfree(i);
		if(targ){
			printf("Found a spot! i = %u\n", i);
			*targ = mine;
		} else {
			printf("Cannot find a spot! i = %u\n", i);
		}
	}
	for(unsigned int i = 1; i < 34120; i+= 74){
		hdata* targ = myhmap_get(i);
		if(targ){
			printf("Found him! i = %u, data = %u, id = %u\n", i, targ->data, targ->id);
			targ->id = 0; //clear the spot.
		} else {
			printf("Cannot find my boy! i = %u\n", i);
		}
	}

	for(unsigned int i = 1; i < 34120; i+= 74){
		hdata* targ = myhmap_get(i);
		if(targ)
			puts("Bad juju!");
	}
}

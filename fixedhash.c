#include "fixedmem.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct {
	size_t id; //Required by hashmap. note that id = 0 is an invalid hash value, it registers as "unused."
	unsigned int data;
} hdata;

//typename, name of the hashmap, power of 2 of the width, depth.
//Argument 1: The struct
//Argument 2: The name of the hashmap.
//Argument 3: the power of 2 of the hashmap's width
//Argument 4: the depth (not power of 2) of the hashmap
//Argument 5: The number of "columns" of the hashmap to search. if it's equal to 1<<(argument 3) then it is most
//memory efficient, but the worst case scenario is slowest.
FIXEDMEM_HASHMAP(hdata, myhmap, 8, 4, 2);
FIXEDMEM_HASHMAP_EXTERN(hdata, myhmap, 8, 4, 2);

int main(){
	for(size_t i = 1; i < 9*256; i+= 256){
		hdata mine;
		mine.id = i;
		mine.data = rand();
		hdata* targ = myhmap_getfree(i);
		if(targ){
			printf("Found a spot! i = %zu\n", i);
			*targ = mine;
		} else {
			printf("Cannot find a spot! i = %zu\n", i);
		}
	}
	for(size_t i = 1; i < 9*256; i+= 256){
		hdata* targ = myhmap_get(i);
		if(targ){
			printf("Found him! i = %zu, data = %u, id = %zu\n", i, targ->data, targ->id);
			targ->id = 0; //clear the spot.
		} else {
			printf("Cannot find my boy! i = %zu\n", i);
		}
	}

	for(size_t i = 1; i < 34120; i+= 74){
		hdata* targ = myhmap_get(i);
		if(targ)
			puts("Bad juju!");
	}
}

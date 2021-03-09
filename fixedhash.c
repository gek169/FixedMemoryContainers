#include "fixedmem.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct {
	size_t id; //Required by hashmap. note that id = 0 is an invalid hash value, it registers as "unused."
	size_t data;
} hdata;

//typename, name of the hashmap, power of 2 of the width, depth.
//Argument 1: The struct
//Argument 2: The name of the hashmap.
//Argument 3: the power of 2 of the hashmap's width
//Argument 4: the depth (not power of 2) of the hashmap
//Argument 5: The number of "columns" of the hashmap to search. if it's equal to 1<<(argument 3) then it is most
//memory efficient, but the worst case scenario is slowest.
//ZERO id is an invalid hashmap entry
FIXEDMEM_HASHMAP(hdata, myhmap, 8, 2, 2);
FIXEDMEM_HASHMAP_EXTERN(hdata, myhmap, 8, 2, 2);

int main(){
	for(size_t i = 1; i < 256*2+7; i++){
		hdata mine;
		mine.id = i + 3109839;
		mine.data = i<<1;
		hdata* targ = myhmap_getfree(mine.id);
		if(targ){
			*targ = mine;
		} else {
			printf("Cannot find a spot! i = %zu\n", i);
		}
	}
	for(size_t i = 1; i < 256 *2+7; i++){
		hdata* targ = myhmap_get(i + 3109839);
		if(targ){
			//printf("Found him! i = %zu, data = %u, id = %zu\n", i, targ->data, targ->id);
			if(i<<1 != targ->data) {printf("This is incorrect.");exit(1);}
			targ->id = 0; //clear the spot.
		} else {
			printf("Cannot find my boy! i = %zu\n", i);
		}
	}

	for(size_t i = 1; i < 256 *2+7; i++){
		hdata* targ = myhmap_get(i);
		if(targ)
			puts("Bad juju!\n");
	}
}

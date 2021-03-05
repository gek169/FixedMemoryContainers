#include "fixedmem.h"
#include <stdio.h>

//28 = 2^10 * 2^10 * 2^8, or 256 megabytes.
FIXEDMEM_BLOCK(myblock, 28);


int main(){
	while(1)
		for(size_t i = 0; i < myblock_size / (sizeof(int)); i++){
			printf("Value at %zx is %d\n", i, ((int*)myblock_st(0))[i]  );
			((int*)myblock_st(0))[i] = i;
		}
}

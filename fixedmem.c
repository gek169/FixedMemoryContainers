#include "fixedmem.h"
#include <stdio.h>

//28 = 2^10 * 2^10 * 2^8, or 256 megabytes.
FIXEDMEM_BLOCK_EXTERN(myblock, 28);
FIXEDMEM_BLOCK(myblock, 28);


int main(){
	while(1)
		for(size_t i = 0; i < myblock_size / (sizeof(double)); i++){
			printf("Value at %zx is %f\n", i, ((double*)myblock(0))[i]  );
			((double*)myblock(0))[i] = (double)i;
		}
}

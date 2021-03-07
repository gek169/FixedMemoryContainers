#include "fixedmem.h"
#include <stdio.h>

//name, power of 2. MUST be large enough to hold at least one size_t.
FIXEDMEM_BLOCK_EXTERN(myblock, 18);
FIXEDMEM_BLOCK(myblock, 18);


int main(){
	//Check memory alignment.
	if(0){
		printf("Array:, %zx\n", (size_t)myblock_mem);
		exit(1);
	}
	while(1)
		for(size_t i = 0; i < myblock_size / (sizeof(double)); i++){
			printf("Value at %zx is %f\n", i, ((double*)myblock(0))[i]  );
			((double*)myblock(0))[i] += (double)i;
		}
}

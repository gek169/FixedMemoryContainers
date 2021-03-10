#include "bigstack.h"
#include <stdio.h>
#include <stdint.h>

BIG_STACK_EXTERN(mystack, 1024);
BIG_STACK(mystack, 1024);

int main(int argc, char** argv){
	size_t s = (size_t)((size_t)1024 * 1024 * 1024 * 24) / BIGSTACK_ALIGNMENT; //24 MB
	mystack_init();
	if(argc > 1){
		s = strtoull(argv[1],0,10);
	}
	char* p;
	for(int i = 0; i < 100; i++){
		p = mystack_push(10);
		printf("\nAddress: %zx\n", (size_t)p);
		if((uintptr_t)p & (uintptr_t)15)
		{puts("\nImproperly aligned.\n");exit(1);}
	}
	mystack_pop(1000);
	p = mystack_push(s);
	for(size_t i = 0; i < (s * BIGSTACK_ALIGNMENT); i++){
		p[i] = i&64?'a'+(i%26):'A'+(i%26);
	}
	p[(s*BIGSTACK_ALIGNMENT)-1] = '\0';
	printf("\n%s\n", p);
	mystack_destroy();
}

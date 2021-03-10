#include <stdlib.h>
#include <stdint.h>

//AN ENORMOUS STACK FOR YOUR ENTIRE PROGRAM.

#ifndef BIGSTACK_SIZE_T
#define BIGSTACK_SIZE_T size_t
#endif

//Guarantee alignment on all platforms- BEEGSTACK grows by this many bytes per push.
#ifndef BIGSTACK_ALIGNMENT
#define BIGSTACK_ALIGNMENT 16
#endif

#define BIGSTACK_ALIGNMENT_M1 (BIGSTACK_ALIGNMENT - 1)
#define BIGSTACK_ALIGNMENT_MASK (~((BIGSTACK_SIZE_T)BIGSTACK_ALIGNMENT_M1))

#define BIG_STACK(name, defsize)\
unsigned char* name##_mem = NULL;\
BIGSTACK_SIZE_T name##_sp = 0;\
BIGSTACK_SIZE_T name##_cap = 0;

#define BIG_STACK_EXTERN(name, defsize)\
extern unsigned char* name##_mem;\
extern BIGSTACK_SIZE_T name##_sp;\
extern BIGSTACK_SIZE_T name##_cap;\
static void name##_init(){\
	name##_mem = realloc(NULL, defsize);\
	name##_cap = defsize;\
}\
static void name##_destroy(){\
	if(name##_mem)free(name##_mem);\
}\
static void* name##_push(BIGSTACK_SIZE_T s){\
	void* retval;\
	s*=BIGSTACK_ALIGNMENT;\
	if((name##_sp + s) < name##_cap){\
		retval = name##_mem + name##_sp;\
		name##_sp += s;\
		return retval;\
	} else {\
		while(s + name##_sp >= name##_cap){\
			name##_cap <<=1 ; /*Grow the beegstack*/\
			name##_mem = realloc(name##_mem, name##_cap);\
		}\
		retval = name##_mem + name##_sp;\
		name##_sp += s;\
		return retval;\
	}\
}\
static inline void name##_pop(BIGSTACK_SIZE_T s) {s *= BIGSTACK_ALIGNMENT; if(name##_sp >= s) name##_sp -= s; else name##_sp = 0;}

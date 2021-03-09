//PROTOTYPE FOR THE COMMON CASE OF A TREE OR VECTOR TO MANAGE MEMORY ALLOCATIONS.
//For dynamic memory management.

#include <stdint.h>
#include <stdlib.h>

#ifndef DYNTREE_SIZE_T
#define DYNTREE_SIZE_T size_t
#endif

#ifndef DYNTREE_FREE
#define DYNTREE_FREE free
#endif

#define DYNTREE(type, name, n)							\
typedef struct{											\
	type* d;											\
	void* c[n];									\
} name;													\
static void name##_init(name* f){						\
	f->d = NULL;										\
	for(DYNTREE_SIZE_T i = 0; i < n; i++)						\
		f->c[i] = NULL;							\
}														\
/*Tree traversal with constant memory usage.*/				\
/*Optimized for memory usage, not speed*/					\
static void name##_cleanup(name* f){						\
	name* c = f;											\
	name* cpr = NULL;										\
	DYNTREE_SIZE_T cpi = 0;											\
	while(1){												\
		DYNTREE_SIZE_T i;int hadc = 0;						\
		for(i = 0; i < n; i++)								\
		if(c->c[i])									\
		{cpi = i; cpr = c; c = c->c[i];hadc = 1;break;}	\
		if(hadc)continue;							\
		/*Bottom of the tree. no c.*/				\
		if(cpr)cpr->c[cpi]	= NULL;					\
		if(c->d)DYNTREE_FREE(c->d);							\
		if(c != f) DYNTREE_FREE(c); else break;						\
		c = f; cpr = NULL; cpi = 0;							\
	}														\
															\
}


//PROTOTYPE FOR THE COMMON CASE OF A TREE OR VECTOR OR LINKED LIST TO MANAGE MEMORY ALLOCATIONS.
//For dynamic memory management.

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#ifndef DYNTREE_SIZE_T
#define DYNTREE_SIZE_T size_t
#endif

#ifndef DYNTREE_FREE
#define DYNTREE_FREE free
#endif

#ifndef DYNTREE_ALLOC
#define DYNTREE_ALLOC malloc
#endif


#define BLOCK(type, name, n)\
typedef struct{type d[ ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) ];} name;\
static const DYNTREE_SIZE_T name##_size = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1));\
static void name##_init(name* f){\
	assert(n > 0);\
}\
static type* name##_get(name* f, DYNTREE_SIZE_T i){/*Safe indexing only.*/\
	i &= ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
	return f->d + i;\
}\
static void name##_cleanup(name* f){\
	assert(n > 0);\
}


#define TABLE(type, name, n)\
typedef struct{type* d[ ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) ];} name;\
static const DYNTREE_SIZE_T name##_size = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1));\
static void name##_init(name* f){\
	assert(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++)\
		f->d[i] = NULL;\
}\
static type* name##_lazy_get(name* f, DYNTREE_SIZE_T i){/*Allocates if not available*/\
	i &= ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
	if(f->d[i]) return f->d[i];\
	f->d[i] = (type*)DYNTREE_ALLOC(sizeof(type));\
	if(!f->d[i]) abort();/*Check that memory allocation succeeded- abort on failure.*/\
	memset(f->d[i], 0, sizeof(type)); /*Set to zero.*/\
	return f->d[i];\
}\
static type* name##_get(name* f, DYNTREE_SIZE_T i){/*Safe indexing only.*/\
	i &= ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
	return f->d[i];\
}\
static void name##_cleanup(name* f){\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++){\
		if(f->d[i]) DYNTREE_FREE(f->d[i]);\
	}\
}


#define DYNTREE(type, name, n)							\
typedef struct{											\
	type* d;											\
	void* c[n];											\
} name;													\
static void name##_init(name* f){						\
	f->d = NULL;										\
	for(DYNTREE_SIZE_T i = 0; i < n; i++)				\
		f->c[i] = NULL;										\
}															\
/*Tree traversal with constant memory usage.*/				\
/*Optimized for memory usage, not speed*/					\
static void name##_cleanup(name* f){						\
	name* c = f;											\
	name* cpr = NULL;										\
	DYNTREE_SIZE_T cpi = 0;									\
	while(1){												\
		DYNTREE_SIZE_T i;int hadc = 0;						\
		for(i = 0; i < n; i++)								\
		if(c->c[i])											\
		{cpi = i; cpr = c; c = c->c[i];hadc = 1;break;}		\
		if(hadc)continue;									\
		/*Bottom of the tree. no c.*/						\
		if(cpr)cpr->c[cpi]	= NULL;							\
		if(c->d)DYNTREE_FREE(c->d);							\
		if(c != f) DYNTREE_FREE(c); else break;				\
		c = f; cpr = NULL; cpi = 0;							\
	}														\
															\
}


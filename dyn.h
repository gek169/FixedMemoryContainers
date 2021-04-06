//PROTOTYPE FOR THE COMMON CASE OF A TREE OR VECTOR OR LINKED LIST TO MANAGE MEMORY ALLOCATIONS.
//For dynamic memory management.

#ifndef DYN_H
#define DYN_H


#include <stdint.h>
#include <stdlib.h>

#ifndef DYNTREE_SIZE_T
#define DYNTREE_SIZE_T size_t
#endif

#ifdef DYNTREE_DEBUG

#ifndef DYNTREE_ASSERT
#include <assert.h>
#define DYNTREE_ASSERT(a) assert(a)
#endif

#else
#define DYNTREE_ASSERT(a) /*a comment*/
#endif

#ifndef DYNTREE_FREE
#define DYNTREE_FREE free
#endif

#ifndef DYNTREE_ALLOC
#define DYNTREE_ALLOC malloc
#endif


#define BLOCK(type, name, n, constructor, destructor)\
typedef struct{type d[ ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) ];} name;\
static const DYNTREE_SIZE_T name##_size = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1));\
static const DYNTREE_SIZE_T name##_mask = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1))-1;\
static void name##_init(name* f){\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < name##_size; i++)\
		constructor(f->d + i);\
}\
static type* name##_get(name* f, DYNTREE_SIZE_T i){/*Safe indexing only.*/\
	i &= name##_mask;\
	return f->d + i;\
}\
static DYNTREE_SIZE_T name##_getsize(name *f){return name##_size;}\
static void name##_cleanup(name* f){\
	/*Nothing needs to be done. This type has no dynamic memory usage.*/\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < name##_size; i++)\
		destructor(f->d + i);\
}



/*Replacement for table if you've written code to use TABLE but want BLOCK behavior.*/
#define TABLE_CONST(type, name, n, constructor, destructor, flood)\
typedef struct{type d[ ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) ];} name;\
static const DYNTREE_SIZE_T name##_size = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1));\
static const DYNTREE_SIZE_T name##_mask = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
static void name##_init_flood(name* f){\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++){\
		constructor(f->d +i);\
		flood(f->d +i);\
	}\
}\
static void name##_init(name* f){\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++){\
		constructor(f->d +i);\
		flood(f->d +i);\
	}\
}\
static void name##_flood(name* f){\
	DYNTREE_ASSERT(n > 0);\
	/*Do nothing, already flooded*/\
}\
static DYNTREE_SIZE_T name##_getsize(name *f){return name##_size;}\
static type* name##_lazy_get(name* f, DYNTREE_SIZE_T i){/*Allocates if not available*/\
	i &= name##_mask;\
	return f->d + i;\
}\
static type* name##_get(name* f, DYNTREE_SIZE_T i){/*Safe indexing only.*/\
	i &= name##_mask;\
	return f->d + i;\
}\
static void name##_remove(name* f, DYNTREE_SIZE_T i){\
	i &= name##_mask;\
		{destructor(f->d +i);}\
}\
static void name##_cleanup(name* f){\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++)\
		name##_remove(f, i);\
	*f = {0};\
}



#define TABLE(type, name, n, constructor, destructor, flood)\
typedef struct{type* d[ ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) ];} name;\
static const DYNTREE_SIZE_T name##_size = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1));\
static const DYNTREE_SIZE_T name##_mask = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
static void name##_init_flood(name* f){\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++){\
		f->d[i] = (type*)DYNTREE_ALLOC(sizeof(type));\
		if(!f->d[i]) abort();/*Check that memory allocation succeeded- abort on failure.*/\
		constructor(f->d[i]);\
		flood(f->d[i]);\
	}\
}\
static void name##_init(name* f){\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++)\
		f->d[i] = NULL;\
}\
static DYNTREE_SIZE_T name##_getsize(name *f){return name##_size;}\
static type* name##_lazy_get(name* f, DYNTREE_SIZE_T i){/*Allocates if not available*/\
	i &= name##_mask;\
	if(f->d[i]) return f->d[i];\
	f->d[i] = (type*)DYNTREE_ALLOC(sizeof(type));\
	if(!f->d[i]) abort();/*Check that memory allocation succeeded- abort on failure.*/\
	constructor(f->d[i]);\
	return f->d[i];\
}\
static void name##_flood(name* f){\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++)\
	{/*Perform a flood on the tree- allocate all memory, use the maximum possible amount.*/\
			if(f->d[i]) {flood(f->d[i]);continue;}\
			f->d[i] = (type*)DYNTREE_ALLOC(sizeof(type));\
			if(!f->d[i]) abort();/*Check that memory allocation succeeded- abort on failure.*/\
			constructor(f->d[i]);\
			flood(f->d[i]);\
	}\
}\
static type* name##_get(name* f, DYNTREE_SIZE_T i){/*Safe indexing only.*/\
	i &= name##_mask;\
	return f->d[i];\
}\
static void name##_remove(name* f, DYNTREE_SIZE_T i){\
	i &= name##_mask;\
	if(f->d[i]) {destructor(f->d[i]); DYNTREE_FREE(f->d[i]); f->d[i] = NULL;}\
}\
static void name##_cleanup(name* f){\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++)\
		name##_remove(f, i);\
}


#define DYNTREE(type, name, n, destructor)				\
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
		if(c->d){destructor(c->d); DYNTREE_FREE(c->d);}		\
		if(c != f) DYNTREE_FREE(c); else break;				\
		c = f; cpr = NULL; cpi = 0;							\
	}														\
															\
}


//dyn_h include guard.
#endif

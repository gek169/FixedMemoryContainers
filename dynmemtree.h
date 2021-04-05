//PROTOTYPE FOR THE COMMON CASE OF A TREE OR VECTOR OR LINKED LIST TO MANAGE MEMORY ALLOCATIONS.
//For dynamic memory management.

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


#ifndef DYNTREE_REALLOC
#define DYNTREE_REALLOC realloc
#endif

#define BLOCK(type, name, n, constructor, destructor)\
typedef struct{type d[ ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) ];} name;\
static const DYNTREE_SIZE_T name##_size = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1));\
static void name##_init(name* f){\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < name##_size; i++)\
		constructor(f->d + i);\
}\
static type* name##_get(name* f, DYNTREE_SIZE_T i){/*Safe indexing only.*/\
	i &= ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
	return f->d + i;\
}\
static DYNTREE_SIZE_T name##_getsize(name *f){return name##_size;}\
static void name##_cleanup(name* f){\
	/*Nothing needs to be done. This type has no dynamic memory usage.*/\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < name##_size; i++)\
		destructor(f->d + i);\
}

#define DYNBLOCK(type, name, constructor, destructor)\
typedef struct{type* d; DYNTREE_SIZE_T pow2size;} name;\
static void name##_init(name* f, DYNTREE_SIZE_T initsize){\
	DYNTREE_ASSERT(initsize > 0);\
	f->d = DYNTREE_REALLOC(NULL, ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(initsize-1)) * sizeof(type));\
	if(!f->d) abort();\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(initsize-1)); i++)\
		constructor(f->d + i);\
	f->pow2size = initsize;\
}\
static void name##_resize(name* f, DYNTREE_SIZE_T initsize){\
	if(initsize == f->pow2size) return;\
	if(initsize < f->pow2size){/*New size is smaller than old size.*/\
		for(DYNTREE_SIZE_T i = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(initsize-1));\
		i<((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(f->pow2size-1));\
		i++)\
			destructor(f->d + i);\
	}\
	f->d = DYNTREE_REALLOC(f->d, ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(initsize-1))  * sizeof(type));\
	if(!f->d) abort();\
	if(initsize > f->pow2size){/*New size is larger than old size.*/\
		for(DYNTREE_SIZE_T i = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(f->pow2size-1));\
		i<((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(initsize-1));\
		i++)\
			constructor(f->d + i);\
	}\
	f->pow2size = initsize;\
}\
static type* name##_get(name* f, DYNTREE_SIZE_T i){\
	i &= ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(f->pow2size-1)) - 1;\
	return f->d + i;\
}\
static DYNTREE_SIZE_T name##_getsize(name *f){\
	return ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(f->pow2size-1));\
}\
static void name##_cleanup(name* f){\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(f->pow2size-1)); i++)\
		destructor(f->d + i);\
	DYNTREE_FREE(f->d);\
}

#define TABLE(type, name, n, constructor, destructor)\
typedef struct{type* d[ ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) ];} name;\
static const DYNTREE_SIZE_T name##_size = ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1));\
static void name##_init(name* f){\
	DYNTREE_ASSERT(n > 0);\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++)\
		f->d[i] = NULL;\
}\
static DYNTREE_SIZE_T name##_getsize(name *f){return name##_size;}\
static type* name##_lazy_get(name* f, DYNTREE_SIZE_T i){/*Allocates if not available*/\
	i &= ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
	if(f->d[i]) return f->d[i];\
	f->d[i] = (type*)DYNTREE_ALLOC(sizeof(type));\
	if(!f->d[i]) abort();/*Check that memory allocation succeeded- abort on failure.*/\
	constructor(f->d[i]);\
	return f->d[i];\
}\
static type* name##_get(name* f, DYNTREE_SIZE_T i){/*Safe indexing only.*/\
	i &= ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
	return f->d[i];\
}\
static void name##_remove(name* f, DYNTREE_SIZE_T i){\
	i &= ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)) - 1;\
	if(f->d[i]) destructor(f->d[i]);\
}\
static void name##_cleanup(name* f){\
	for(DYNTREE_SIZE_T i = 0; i < ((DYNTREE_SIZE_T)1<<(DYNTREE_SIZE_T)(n-1)); i++){\
		if(f->d[i]) destructor(f->d[i]);\
		if(f->d[i]) DYNTREE_FREE(f->d[i]);\
	}\
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


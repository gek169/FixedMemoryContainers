//PROTOTYPE FOR THE COMMON CASE OF A TREE OR VECTOR TO MANAGE MEMORY ALLOCATIONS.
//For dynamic memory management.

#include <stdint.h>
#include <stdlib.h>

#define ASSERT_SAME_TYPE(a, b)  ((&(a) == &(b)))

#define HAS_ITEM(A,B,C,D) 				\
static C* buildt_##A##B##D(){ 	\
  A t; 									\
  C q;									\
  int f = ASSERT_SAME_TYPE(q, t.B);		\
  C* ret = &( t . B );					\
  return ret;							\
}


#define DYNTREE(type, name, n)							\
typedef struct{											\
	type* data;											\
	void* children[n];									\
} name;													\
static void name##_init(name* f){						\
	f->data = NULL;										\
	for(size_t i = 0; i < n; i++)						\
		f->children[i] = NULL;							\
}														\
/*Tree traversal with constant memory usage.*/				\
/*Optimized for memory usage, not speed*/					\
static void name##_cleanup(name* f){						\
	name* c = f;											\
	name* cpr = NULL;										\
	size_t cpi = 0;											\
	while(1){												\
		size_t i;int hadchildren = 0;						\
		for(i = 0; i < n; i++)								\
		if(c->children[i])									\
		{cpi = i; cpr = c; c = c->children[i];hadchildren = 1;break;}	\
		if(hadchildren)continue;							\
		/*Bottom of the tree. no children.*/				\
		if(cpr)cpr->children[cpi]	= NULL;					\
		if(c->data)free(c->data);							\
		if(c != f) free(c); else break;						\
		c = f; cpr = NULL; cpi = 0;							\
	}														\
															\
}															\


//FIXED MEMORY IMPLEMENTATION, PLUS CONTAINERS AND EXTRAS
// Authored by Gek/ DMHSW For the Public Domain, CC0 license.

// Fixed size memory containers for C

//This is useful for many reasons:
//1) You can easily serialize your pointers and the entire memory of your program.
//2) No reliance on malloc and free.

//This is the must-have library for the embedded C programmer, or C programming hobbyist!

#include <stdint.h>
#include <stdlib.h>
#ifndef FIXEDMEM_NO_ALIGN
#include <stdalign.h>
#define FIXEDMEM_ALIGN alignas(256)
#else
#define FIXEDMEM_ALIGN /* a comment*/
#endif

#ifndef FIXEDMEM_NO_UINTPTR_T
#define FIXEDMEM_PTR_UINT uintptr_t
#else
#define FIXEDMEM_PTR_UINT size_t
#endif

#ifndef FIXEDMEM_SIZE_T
#define FIXEDMEM_SIZE_T size_t
#endif

#define ASSERT_SAME_TYPE(a, b)  ((&(a) == &(b)))

#define HAS_ITEM(A,B,C,D) 				\
static  C* buildt_##A##B##D(){ 	\
  A t; 									\
  C q;									\
  int f = ASSERT_SAME_TYPE(q, t.B);		\
  C* ret = &( t . B );					\
  return ret;							\
}


//The basic fixed memory block!
//Or "vector" if you want to get all computer-sciencey
//It's aligned way beyond what you need for your processor, so it can store any type.
//As long as the pointers you use in your program are aligned, so will the "real" pointers obtained by offsetting by name##_mem
#define FIXEDMEM_BLOCK(name, pow2)\
FIXEDMEM_ALIGN unsigned char name ## _mem [(((FIXEDMEM_SIZE_T)1)<<pow2)];

#define FIXEDMEM_BLOCK_EXTERN(name, pow2)\
FIXEDMEM_ALIGN extern unsigned char name ## _mem [(((FIXEDMEM_SIZE_T)1)<<pow2)];\
static const FIXEDMEM_SIZE_T name ## _size = ((FIXEDMEM_SIZE_T)1)<<pow2;\
static  void* name(void* p) {return (void*)((FIXEDMEM_PTR_UINT)( name ## _mem ) + (FIXEDMEM_PTR_UINT)p); }\
static  void* name##_st(FIXEDMEM_SIZE_T p) {return (void*)(name ## _mem  + p); } /*We assume they want the fast access.*/


//The hash map!

//Zero is an invalid hash value.
#define FIXEDMEM_HASHMAP(type, name, pow2width, depth, alt)							\
type name##_mem[ (((FIXEDMEM_SIZE_T)1)<<pow2width) * depth ];


#define FIXEDMEM_HASHMAP_EXTERN(type, name, pow2width, depth, alt)			\
static const FIXEDMEM_SIZE_T name ##_width = (((FIXEDMEM_SIZE_T)1)<<pow2width);				\
static const FIXEDMEM_SIZE_T name ##_mask = (((FIXEDMEM_SIZE_T)1)<<pow2width) - 1;			\
extern type name##_mem[ (((FIXEDMEM_SIZE_T)1)<<pow2width) * depth ];					\
static  type* name##_get(FIXEDMEM_SIZE_T id){										\
	for(FIXEDMEM_SIZE_T attempt = 0; attempt < alt; attempt++){				\
			type* retval = name##_mem + ((id &  name ##_mask ) +attempt) * depth;\
			for(FIXEDMEM_SIZE_T i = 0; i < depth; i++){					\
				if(retval->id == id)							\
					return retval;								\
				retval++;										\
			}													\
		}														\
	return NULL;											\
}															\
static  type* name##_getfree(FIXEDMEM_SIZE_T id){					\
	for(FIXEDMEM_SIZE_T attempt = 0; attempt < alt; attempt++){				\
		type* retval = name##_mem + ((id &  name ##_mask ) +attempt) * depth;\
		for(FIXEDMEM_SIZE_T i = 0; i < depth; i++){					\
			if(retval->id == 0)								\
				return retval;								\
			retval++;										\
		}													\
	}														\
	return NULL;											\
}														



//The linked list!

//Indices beyond the maximum extent return NULL when you try to get them.
//Indices are Lua-style, 0 is the invalid index, internally.
//Externally, the user should experience the library as if 0-based indices are used.
#define FIXEDMEM_LL(type, name, pow2size)									\
type name##_mem[((FIXEDMEM_SIZE_T)1)<<pow2size];/*Initialized to zero.*/				\
FIXEDMEM_SIZE_T name ## _head = 0; /*The head of the linked list. 0 means its empty.*/	

//External declaration
#define FIXEDMEM_LL_EXTERN(type, name, pow2size)									\
static const FIXEDMEM_SIZE_T name##_size = ((FIXEDMEM_SIZE_T)1)<<pow2size;					\
static const FIXEDMEM_SIZE_T name##_mask = (((FIXEDMEM_SIZE_T)1)<<pow2size) - 1;				\
extern type name##_mem[((FIXEDMEM_SIZE_T)1)<<pow2size];/*Initialized to zero.*/				\
extern FIXEDMEM_SIZE_T name ## _head; /*The head of the linked list. 0 means its empty.*/	\
static FIXEDMEM_SIZE_T name##_internal_getfree(){/*Find a free spot*/					\
	for(FIXEDMEM_SIZE_T i = 0; i < name##_size; i++)	/*Linearly search for free spot*/\
		if(name##_mem[i].used == 0) return i+1; /*Lua indexing*/			\
	return name##_size+1;													\
}																			\
static  type* name(FIXEDMEM_SIZE_T index){/*Traverse the linked list*/		\
	/*The user has entered a zero index.*/									\
	FIXEDMEM_SIZE_T t = (name##_head-1);									\
	for(FIXEDMEM_SIZE_T i = 0; i < index; i++) 								\
		if(t > name##_mask || name##_mem[t].used == 0){ 					\
			return NULL; 													\
		}else{ /*Don't follow a link from an bad/unused node*/				\
			t = name##_mem[t].next;											\
 			if(t==0)return NULL;											\
 			t--;															\
 		}/*Follow the link*/												\
	if(name##_mem[t].used == 0) return NULL; /* Need an additional test*/	\
	return name##_mem + t;													\
}																			\
static  FIXEDMEM_SIZE_T name##_remove(FIXEDMEM_SIZE_T index){							\
	/*User has entered a zero-based index.*/								\
	type* atind = name(index);												\
	if(!atind) return 0; /*Error- invalid index.*/							\
	atind->used = 0; /* No longer being used.*/								\
	if(index == 0){ /*Pop the head. NOT LUA INDEXED!!!*/					\
		name##_head = atind->next; atind->next = 0; /*Done*/				\
		return 1;															\
	}																		\
	/*we must set the previous index's next */								\
	{type* atprevind = name(index - 1);if(!atprevind) return 0;/*Error*/	\
		atprevind->next = atind->next; 										\
		atind->next = 0;return 1; /*Success!*/								\
	}																		\
}																			\
static  FIXEDMEM_SIZE_T name##_insert(FIXEDMEM_SIZE_T index, type me){					\
	FIXEDMEM_SIZE_T di = name##_internal_getfree();/*Destination Index. it is lua-indexed.*/		\
	me.used = 1;/*The user will not have set it*/							\
	if(di > name##_size) return 0;	/*Linked list is full!*/				\
	name##_mem[di-1] = me;	/*Found the location to place our thing.*/		\
	if(index == 0){	/*Desires to be the new head. First to go fast.*/		\
		name##_mem[di-1].next = name##_head;								\
		name##_head = di;	return 1; /*Lua Index.*/										\
	} else {	/*The previous element's next must  be set*/				\
		type* bruh = name(index-1);	/*Previous element*/					\
		if(!bruh) return 0; /*Error!*/										\
		FIXEDMEM_SIZE_T bruh_next_old = bruh->next;/*Get his next*/			\
		bruh->next = di;/*Set his next this is a LUA INDEX*/				\
		name##_mem[di-1].next = bruh_next_old; /*Set our next*/				\
		return 1; /*Success!*/												\
	}																		\
	return 0;/*Some failure?*/												\
}

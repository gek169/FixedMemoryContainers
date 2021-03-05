//FIXED MEMORY IMPLEMENTATION, PLUS CONTAINERS AND EXTRAS
// Authored by Gek/ DMHSW For the Public Domain

//Allows you to declare contiguous blocks of memory

//This is useful for many reasons:
//1) You can easily serialize your pointers and the entire memory of your program.
//2) 

#include <stdint.h>
#include <stdlib.h>

//EXPLANATION
#define ASSERT_SAME_TYPE(a, b)  ((&(a) == &(b)))

#define HAS_ITEM(A,B,C,D) \
static inline C* buildt_##A##B##D(){ \
  A t; \
  C q;\
  int f = ASSERT_SAME_TYPE(q, t.B);\
  C* ret = &( t . B );\
  return ret;\
}


//The basics
#define FIXEDMEM_BLOCK_EXTERN(name, pow2)\
extern uint8_t name ## _mem [((size_t)1)<<pow2];\
static const size_t name ## _size = ((size_t)1)<<pow2;\
static const size_t name ## _mask = ((size_t)1)<<pow2 -1;\
static inline void* name(void* p) {return (void*)((size_t)( name ## _mem ) + (size_t)p); }\
static inline void* name##_st(size_t p) {return (void*)(( name ## _mem ) + p); }

#define FIXEDMEM_BLOCK(name, pow2)\
uint8_t name ## _mem [((size_t)1)<<pow2];\
static const size_t name ## _size = ((size_t)1)<<pow2;\
static const size_t name ## _mask = ((size_t)1)<<pow2 -1;\
static inline void* name(void* p) {return (void*)((size_t)( name ## _mem ) + (size_t)p); }\
static inline void* name##_st(size_t p) {return (void*)(( name ## _mem ) + p); }

//Zero is an invalid hash value.
#define FIXEDMEM_HASHMAP(type, name, pow2width, depth)							\
HAS_ITEM(type, id, size_t, fixedmem_hashmap_type_has_id_property_test)	\
static const size_t name ##_width = (((size_t)1)<<pow2width);		\
static const size_t name ##_mask = (((size_t)1)<<pow2width) - 1;	\
type name##_mem[ (((size_t)1)<<pow2width) * depth ];	\
static inline type* name##_get(size_t id){		\
	type* retval = name##_mem + (id & ( name ##_mask ));\
	size_t cdep = 0;									\
	for(size_t i = 0; i < depth; i++){					\
		if(retval->id == id)							\
			return retval;								\
		retval += name ##_width;						\
	}													\
	return NULL;										\
}														\
static inline type* name##_getfree(size_t id){	\
	type* retval = name##_mem + (id & ( name ##_mask ));\
	size_t cdep = 0;									\
	for(size_t i = 0; i < depth; i++){					\
		if(retval->id == 0)								\
			return retval;								\
		retval += name ##_width;						\
	}													\
	return NULL;										\
}														

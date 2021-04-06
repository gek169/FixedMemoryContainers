# Safe Memory Management Utilities and Containers for C.

## Usage?

Check the examples.

## What do you have?

Fixed memory containers, which are allocated globally and not typedef'd

* A hashmap of restricted depth with the ability to use alternate columns 

* a way to declare blocks of contiguous memory (Alignment: 256, but you can change or disable it.)

* A linked list

Dynamic memory containers, which are typedef'd (dyn.h)

* An n-tree implementation with an arbitrary number of children decided at compiletime. Not declared as global, but rather typedef'd

* table. a safely-indexed array of pointers with lazy allocation (Allocate-on-get) functionality. 
Size is specified as a power of 2.

* Block. a safely indexed array. Size is specified as a power of two.

* Dynblock. A dynamically sized, safely indexed array which uses realloc. Again, a power of two.

Dynamic memory containers which are not typedef'd but are declared globally:

* the BIG STACK. allows you to push blocks of memory (of size BIGSTACK_ALIGNMENT) or pop them, with impunity.

(A workaround for programs which want a really big stack)


General-purpose macros for creating helper datatypes and/or functions for dynamic memory management (dyn.h)

* CREATE_DESTRUCTOR, which defines a destructor that calls subsequent destructors for a struct.

* PTR, which defines wrapped pointer types that retain size and borrowing information in the C type system.


## Does the world need another container library for C?

Previously, I thought not. But yes, it does. C++ is far too slow and generates HORRIBLE code,

Github projects are extremely sparse and a lot of them are extremely intrusive and hard to use.

This library provides reasonably non-intrusive, minimal solutions which (I hope) blend well with other C code.

## Have you tested these?

Reasonably extensively. All test cases tried are included with the repository.

## Why not use C++?

Like I said. C++? Kind of slow. Lacks portability, and C++ code is EXTREMELY hard to maintain.

C code is far more maintainable, readable, and portable.

I write C++ sometimes, but C is my native programming tongue, I love it.

# Simple, Templated, Intuitive, Reasonably efficient, C++like, Fixed Memory Containers for C99, Public Domain

The fixed memory containers (in fixedmem.h) do not require any malloc or free calls, they perform
their own memory management on fixed-size global arrays. They are very useful for situations where you don't
want non-serializable pointers in your code, or in situations where runtime dynamic memory allocation is not desired.

Other containers in the repository exist purely as STL stand-ins for environments where C++ is either
not practical, not desired, or not available.

Note: In modern operating systems, if you look at the resource usage used by this library, you might see that far less memory
is being used than has been declared. Why? I don't know.

## Usage?

Check the examples.

## What do you have?

Fixed memory containers, which are allocated globally and not typedef'd

* A hashmap of restricted depth with the ability to use alternate columns 

* a way to declare blocks of contiguous memory (Alignment: 256, but you can change or disable it.)

* A linked list

Dynamic memory containers, which are typedef'd

* An n-tree implementation with an arbitrary number of children decided at compiletime. Not declared as global, but rather typedef'd

has a naive (hacky) but non-recursive destructor.



* table. a safely-indexed array of pointers with lazy allocation (Allocate-on-get) functionality. Size is specifies as a power of 2.

* Block. a safely indexed array. Size is specified as a power of two.


Dynamic memory containers which are not typedef'd but are declared globally:

* the BIG STACK. allows you to push blocks of memory (of size BIGSTACK_ALIGNMENT) or pop them, with impunity.

(A workaround for programs which want a really big stack)



## Does the world need another container library for C?

Probably not, but that doesn't make it any less cool huh? 

## Have you tested these?

Test the examples with valgrind. Play around with the numbers. Check to make sure that containers don't
overwrite each other.

In short: Yes, I have.

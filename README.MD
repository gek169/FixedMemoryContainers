# Simple, Templated, Intuitive, C++like, Fixed Memory Containers for C, Public Domain

The fixed memory containers (in fixedmem.h) do not require any malloc or free calls, they perform
their own memory management on fixed-size global arrays.

Other containers in the repository exist purely as STL stand-ins for environments where C++ is either
not practical, not desired, or not available.



This project was originally started to see what would happen if you created an incredibly large
global array. As it turns out, not only will it not actually get allocated into your program
until you use it (Tested using GCC, on x86_64, Debian 10) but it will do so dynamically- Only portions
of the global array exist until the whole thing is allocated.

## Usage?

Check the examples.

## What do you have?

Fixed memory containers:

* A hashmap of restricted depth with the ability to use alternate columns 

* a way to declare blocks of contiguous memory.

* A linked list

Dynamic memory containers:

* An n-tree implementation with an arbitrary number of children decided at compiletime.

has a naive (hacky) but non-recursive destructor.

## Does the world need another container library for C?

Probably not, but that doesn't make it any less cool huh? 

## Have you tested these?

Test the examples with valgrind. Play around with the numbers. Check to make sure that containers don't
overwrite each other.

In short: Yes, I have.

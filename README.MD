# Simple Fixed Memory (Plus some others...) Containers for C, Public Domain

The fixed memory containers (in fixedmem.h) do not require any malloc or free calls, they perform
their own memory management on fixed-size global arrays.

Other containers in the repository exist purely as STL stand-ins.



This project was originally started to see what would happen if you created an incredibly large
global array. As it turns out, not only will it not actually get allocated into your program
until you use it (Using GCC, on x86_64, Debian 10) but it will do so dynamically- Only portions
of the global array exist until the whole thing is allocated.

## Usage?

Check the examples.

## What do you have?

Fixed memory containers:

* A hashmap of restricted depth, 

* a way to declare blocks of contiguous memory.

* A linked list

Dynamic memory containers:

* An n-tree implementation with an arbitrary number of children designed to manage free() ing memory at the end of your program.

## Does the world need another container library for C?

Probably not, but that doesn't make it any less cool huh? 

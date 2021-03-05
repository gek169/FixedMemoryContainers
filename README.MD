# Simple Fixed Memory Containers for C, Public Domain

No malloc, no free, just simple fixed memory containers.

You declare it global and you have access to it everywhere.

This project was originally started to see what would happen if you created an incredibly large
global array. As it turns out, not only will it not actually get allocated into your program
until you use it (Using GCC, on x86_64, Debian 10) but it will do so dynamically- Only portions
of the global array exist until the whole thing is allocated.

## Usage?

Check the examples.

## What do you have?

* A hashmap of restricted depth, 

* a way to declare blocks of contiguous memory.

* A linked list

## Does the world need another container library for C?

Probably not, but that doesn't make it any less cool huh? 

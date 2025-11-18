# Custom Dynamic Memory Allocator 

A collection of C implementations demonstrating how *dynamic memory allocation works inside an operating system*.  
This project recreates the functionality of:

- malloc
- free
- calloc
- realloc

…using custom logic built on top of the **Linux sbrk() system call**.

This allocator uses:

- A linked list of memory blocks
- Metadata headers to track sizes & block status
- Splitting of large blocks
- Coalescing of adjacent free blocks
- First-Fit allocation strategy

Perfect for *OS courses, **systems programming practice, or **low-level memory management study*.

---

# How to Compile and Run

This project includes a Makefile for automatic compilation.

### You will need:
- gcc
- make
- A Linux environment: *Ubuntu, WSL, Debian, Fedora, or macOS*

---

## Step 1: Clone (or download) the project

bash
git clone 
cd malloc-project


---

## Step 2: Build the allocator and test program

bash
make


This produces an executable:


test


---

## Step 3: Run the test suite

bash
./test


---

## Step 4: Clean build files

bash
make clean


---

# The Components

Below are the key source files included in the project.

---

## **1. src/mymalloc.c**
### What It Does
Implements the entire *custom memory allocator*:

- Manages a linked list of heap blocks  
- Requests heap memory using sbrk()  
- Splits large free blocks  
- Coalesces adjacent free blocks  
- Implements First-Fit allocation strategy  

Overrides:

- malloc
- free
- calloc
- realloc

This file contains the *core logic* of the project.

---

## **2. src/mymalloc.h**
### What It Does
Declares all custom memory management function prototypes.  
Included by both:

- mymalloc.c
- test.c

---

## **3. src/test.c**
### What It Demonstrates
A suite of scenarios validating correct allocator behavior:

- Simple malloc + free
- Zero-initialized calloc
- Growing memory regions with realloc
- Splitting oversized blocks
- Coalescing adjacent free blocks
- Allocations after fragmentation

---

## Output
<img width="891" height="487" alt="Result" src="https://github.com/user-attachments/assets/73064a9c-1f00-4132-be31-9228025ee1b4" />

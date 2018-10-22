/*
CSE 109: Fall 2018
Christopher Mallalieu
cpm220
Memmory Allocator
Program #4
*/

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include<stdlib.h>
#include<stdio.h>
#include"Allocation.h"


struct Allocator_t
{
	void* memory;
	size_t capacity;
	struct Allocation_t** allocationList;
	size_t allocationListSize;
	size_t allocationListCapacity;
	size_t memUsed;
};

//create and destroy
void makeAllocator(struct Allocator_t* this, size_t capacity);
void freeAllocator(struct Allocator_t* this);

//allocate and deallocate memory
void* allocate(struct Allocator_t* this, size_t amt);
void deallocate(struct Allocator_t* this, void* ptr);
void* riskyAlloc(struct Allocator_t* this, size_t addSize);

//getters
void* getBase(struct Allocator_t* this);
size_t getUsed(struct Allocator_t* this);
size_t getCapacity(struct Allocator_t* this);
struct Allocation_t* getAllocation(struct Allocator_t* this, size_t index);
size_t numAllocations(struct Allocator_t* this);

#endif


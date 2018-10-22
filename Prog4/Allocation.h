/*
CSE 109: Fall 2018
Christopher Mallalieu
cpm220
Memmory Allocator
Program #4
*/


#ifndef ALLOCATION_H
#define ALLOCATION_H

#include<stdlib.h>

struct Allocation_t
{
	size_t startOffset;
	size_t allocationSize;
};

//create and destroy
void makeAllocation(struct Allocation_t* this, size_t startOffset, size_t allocationSize);
void freeAllocation (struct Allocation_t* this);

//get methods
size_t getStart(struct Allocation_t* this);
size_t getEnd(struct Allocation_t* this);
size_t getSize(struct Allocation_t* this);

//Does memory overlap
int doesOverlap(struct Allocation_t* this, size_t checkOffset, size_t checkAllocationSize);

#endif

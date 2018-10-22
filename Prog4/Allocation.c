/*
CSE 109: Fall 2018
Christopher Mallalieu
cpm220
Memmory Allocator
Program #4
*/

#include "Allocation.h"
#include <stdlib.h>

//create and destroy
void makeAllocation(struct Allocation_t* this, size_t startOffset, size_t allocationSize)
{
	this->startOffset = startOffset;
	this->allocationSize = allocationSize;
}

void freeAllocation (struct Allocation_t* this)
{
	this->startOffset = 0;
	this->allocationSize = 0;
}

//get methods
size_t getStart(struct Allocation_t* this)
{
	return this->startOffset;
}

size_t getEnd (struct Allocation_t* this)
{
	size_t end = getStart(this) + getSize(this);
	return end;
}

size_t getSize(struct Allocation_t* this)
{
	return this-> allocationSize;
}

//Does memory overlap
int doesOverlap(struct Allocation_t* this, size_t checkOffset, size_t checkAllocationSize)
{
	size_t thisOffset = getStart(this);
	size_t thisEnd = getEnd(this);
	size_t checkEnd = checkOffset + checkAllocationSize;
	if (thisOffset >= checkOffset && thisOffset < checkEnd) //if statements to determine if offsets overlap eachother
	{
		return 1; //return 1 if there is overlap
	}
	else if (thisEnd > checkOffset && thisEnd <= checkEnd)
	{
		return 1;
	} 
	else if (thisOffset <= checkOffset && thisEnd >= checkEnd)
	{
		return 1; 
	}
	else
	{
		return 0; //return 0 if there is no overlap
	}
}
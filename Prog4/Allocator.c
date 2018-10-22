/*
CSE 109: Fall 2018
Christopher Mallalieu
cpm220
Memmory Allocator
Program #4
*/

#include "Allocator.h"
#include "Allocation.h"
#include <stdlib.h>

void makeAllocator(struct Allocator_t* this, size_t capacity)
{
	while (capacity % 16 != 0)
	{
		capacity++;
	}
	this->memory = (void*) malloc(capacity);
	this->capacity = capacity;
	this->allocationList = NULL;
	this->allocationListSize = 0;
	this->allocationListCapacity = 0;
	this->memUsed = 0;
}

void freeAllocator(struct Allocator_t* this)
{
	for (int i = 0; i < this->allocationListCapacity; i++)
	{
		free(this->allocationList[i]);
		this->allocationList[i] = NULL;
	}

	free(this->memory);
	this->memory = NULL;
	this->capacity = 0;
	this->allocationList = NULL;
	this->allocationListCapacity = 0;
	this->memUsed = 0;
}

void* allocate(struct Allocator_t* this, size_t amt)
{
	while (amt % 16 != 0)
	{
		amt++;
	}
	
	int overlap = 0;
	for (size_t j = 0; j < getCapacity(this); j+= 16)
	{
		if (j + amt <= getCapacity(this))
		{
			for (size_t k = 0; k < numAllocations(this); k++)
			{
				if (doesOverlap(getAllocation(this, k), j, amt))
				{
					overlap = 1;
				}
			}
			if (!overlap)
			{
				if (numAllocations(this) + 1 > this->allocationListCapacity)
				{
					size_t oldAllocListSize = numAllocations(this);
					this->allocationListCapacity = (oldAllocListSize * 2) + 1;
					struct Allocation_t** newAllocList = (struct Allocation_t**) malloc(this->allocationListCapacity * sizeof(struct Allocation_t*));
					
					for (size_t i = 0; i < this->allocationListCapacity; i++)
					{
						if (i < oldAllocListSize)
						{
							newAllocList[i] = this->allocationList[i];
						}
						else
						{
							newAllocList[i] = NULL;
						}
					}
					free(this->allocationList);
					this->allocationList = newAllocList;
				}
				this->allocationList[numAllocations(this)] = (struct Allocation_t*) malloc(sizeof(struct Allocation_t));
				makeAllocation(this->allocationList[numAllocations(this)], j, amt);
				this->allocationListSize++;
				this->memUsed += amt;
				return getBase(this) + j;
			}
		}
		overlap = 0;
	}
	return NULL;
}

void deallocate(struct Allocator_t* this, void* ptr)
{
	if (ptr != NULL)
	{
		size_t foundFlag = 0;
		for (size_t i = 0; i < numAllocations(this); i++)
		{
			if (getBase(this) + getStart(getAllocation(this, i)) == ptr)
			{
				this->memUsed -= getSize(getAllocation(this, i));
				free(getAllocation(this, i));
				for (int j = i; j < numAllocations(this); j++)
				{
					this->allocationList[j] = getAllocation(this, j + 1);
				}
				this->allocationListSize--;
				foundFlag = 1;
			}
		}
		if (!foundFlag)
		{
			fprintf(stderr, "Corruption in free");
			exit(1);
		}
	}
}

void* riskyAlloc(struct Allocator_t* this, size_t addSize)
{
	void* checkPtr = NULL;

	void* alloc = allocate(this, addSize);
	if (alloc == NULL)
	{
		size_t newSize = getCapacity(this) + addSize;
		checkPtr = realloc(getBase(this), newSize);
	}
	if (checkPtr == getBase(this))
	{
		this->capacity += addSize;
		return getBase(this);
	}
	else
	{
		fprintf(stderr, "Bad realloc");
		return NULL;
	}
	
}

void* getBase(struct Allocator_t* this)
{
	return this->memory;
}

size_t getUsed(struct Allocator_t* this)
{
	return this->memUsed;
}

size_t getCapacity(struct Allocator_t* this)
{
	return this->capacity;
}

struct Allocation_t* getAllocation(struct Allocator_t* this, size_t index)
{
	if (index < 0 || index <= this->allocationListSize)
	{
		return this->allocationList[index];
	}
	return NULL;
} 

size_t numAllocations(struct Allocator_t* this)
{
	return this->allocationListSize;
}


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

void makeAllocator(struct Allocator_t* this, size_t capacity) //constructs allocator struct
{
	while (capacity % 16 != 0) //ensures size is a multiple of 16
	{
		capacity++;
	}
	this->memory = (void*) malloc(capacity); //initialize base settings
	this->capacity = capacity;
	this->allocationList = NULL;
	this->allocationListSize = 0;
	this->allocationListCapacity = 0;
	this->memUsed = 0;
}

void freeAllocator(struct Allocator_t* this) //free allocator struct
{
	for (int i = 0; i < this->allocationListCapacity; i++) //free all allocations inside allocator
	{
		free(this->allocationList[i]);
		this->allocationList[i] = NULL;
	}

	free(this->memory); //free allocator 
	this->memory = NULL;
	this->capacity = 0;
	this->allocationList = NULL;
	this->allocationListCapacity = 0;
	this->memUsed = 0;
}

void* allocate(struct Allocator_t* this, size_t amt)
{
	while (amt % 16 != 0) //ensure amt is a multiple of 16
	{
		amt++;
	}
	
	int overlap = 0; //overlap Flag
	for (size_t j = 0; j < getCapacity(this); j+= 16) //iterate through memory by 16
	{
		if (j + amt <= getCapacity(this)) //if there is space in memory to add
		{
			for (size_t k = 0; k < numAllocations(this); k++) //iterate through allocationList
			{
				if (doesOverlap(getAllocation(this, k), j, amt)) //if it overlaps, set flag to true
				{
					overlap = 1;
				}
			}
			if (!overlap) //if there is no overlap 
			{
				if (numAllocations(this) + 1 > this->allocationListCapacity) //if there is no space in allocationList for another allocation
				{
					size_t oldAllocListSize = numAllocations(this); 
					this->allocationListCapacity = (oldAllocListSize * 2) + 1; //increase the size allocationList and malloc more memory for it
					struct Allocation_t** newAllocList = (struct Allocation_t**) malloc(this->allocationListCapacity * sizeof(struct Allocation_t*));
					
					for (size_t i = 0; i < this->allocationListCapacity; i++) //copy over the data in the old allocationList
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
					free(this->allocationList); //free old allocationList
					this->allocationList = newAllocList;
				}
				this->allocationList[numAllocations(this)] = (struct Allocation_t*) malloc(sizeof(struct Allocation_t)); //create allocation struct and add it to allocationList
				makeAllocation(this->allocationList[numAllocations(this)], j, amt);
				this->allocationListSize++; //keep track of sizes
				this->memUsed += amt;
				return getBase(this) + j; //returns location of new allocation
			}
		}
		overlap = 0;
	}
	return NULL; //if there is no room to add an allocation, return NULL
}

void deallocate(struct Allocator_t* this, void* ptr)
{ 
	if (ptr != NULL) //if the pointer being lookoed for is not NULL
	{
		size_t foundFlag = 0;
		for (size_t i = 0; i < numAllocations(this); i++) //iterate thorugh the allocationList
		{
			if (getBase(this) + getStart(getAllocation(this, i)) == ptr) //if the pointer exists
			{
				this->memUsed -= getSize(getAllocation(this, i)); //keep track of memUsed
				free(getAllocation(this, i)); //free the target allocation
				for (int j = i; j < numAllocations(this); j++) //move the list back to fill the void left by the freed allocation
				{
					this->allocationList[j] = getAllocation(this, j + 1);
				}
				this->allocationListSize--;
				foundFlag = 1;
			}
		}
		if (!foundFlag) //if the target pointer was not found
		{
			fprintf(stderr, "Corruption in free");
			exit(1);
		}
	}
}

void* riskyAlloc(struct Allocator_t* this, size_t addSize) //if there is no memory left to allocate, call realloc and see if it is successful
{
	void* checkPtr = NULL;

	void* alloc = allocate(this, addSize); //first, try and allocate normally
	if (alloc == NULL) //if there is no space to allocate normally
	{
		size_t newSize = getCapacity(this) + addSize; //increase the capacityof memory
		checkPtr = realloc(getBase(this), newSize); //realloc more memory for the pointer
	}
	if (checkPtr == getBase(this)) //if the realloc was successful, return memory
	{
		this->capacity += addSize; //keep track of memory capacity
		return getBase(this);
	}
	else
	{
		fprintf(stderr, "Bad realloc");
		return NULL;
	}
	
}

void* getBase(struct Allocator_t* this) //get memory pointer
{
	return this->memory;
}

size_t getUsed(struct Allocator_t* this) //get memUsed
{
	return this->memUsed;
}

size_t getCapacity(struct Allocator_t* this) //get memory capacity
{
	return this->capacity;
}

struct Allocation_t* getAllocation(struct Allocator_t* this, size_t index) //get allocatoin in the list at index
{
	if (index < 0 || index <= this->allocationListSize) 
	{
		return this->allocationList[index];
	}
	return NULL; //return NULL if there is no allocation at the given index
} 

size_t numAllocations(struct Allocator_t* this) //returns allocationList size
{
	return this->allocationListSize;
}


#ifndef PRINT_ALLOCATIONS_H
#define PRINT_ALLOCATIONS_H

#include<stdlib.h>
#include<stdio.h>
#include"Allocation.h"
#include"Allocator.h"

void printAllocations(struct Allocator_t* it, FILE* fd);

#endif

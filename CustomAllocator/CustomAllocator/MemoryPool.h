#pragma once
#include "stdafx.h"


class MemoryPool
{
public:
	MemoryPool(size_t poolSize);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* /*aFileName*/, int /*aLineNumber*/);
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/);

	void maintainSorted(std::list<PoolElement>::iterator& element);

	~MemoryPool();

public:
	char* startAdress;					// Start adress of the memory pool
	std::list<PoolElement> mAvailable;	// Stores the memory unallocated sorted descending by the size
	std::set<PoolElement> mAllocated;	// Stores the memory allocated sorted by the adress
	std::size_t poolSize;				// The size of the memory pool
	Logger test;
};
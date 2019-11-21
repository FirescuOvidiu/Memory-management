#pragma once
#include "stdafx.h"


/* 
	Represents a memory pool
*/
class MemoryPool
{
public:
	MemoryPool(size_t poolSize);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* /*aFileName*/, int /*aLineNumber*/);
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/);

	~MemoryPool();

private:
	bool checkBadAlloc(size_t aSize);
	void checkMemoryLeaks();
	void maintainSorted(std::list<PoolElement>::iterator& element);
	void insertIntoAvailableMemory(const PoolElement& deletedMemory);

private:
	char* startAddress;					// Start address of the memory pool
	std::list<PoolElement> mAvailable;	// Stores the memory unallocated sorted descending by the size
	std::set<PoolElement> mAllocated;	// Stores the memory allocated sorted by the address
	std::size_t poolSize;				// The size of the memory pool
	Logger log;							// The log of the memory pool
};
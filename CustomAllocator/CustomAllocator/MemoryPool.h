#pragma once
#include "stdafx.h"


class MemoryPool
{
public:
	MemoryPool(size_t poolSize);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	void __cdecl freeMemory(void* aBlock, int aBlockUse);

	void maintainSorted(std::list<PoolElement>::iterator& element)
	{
		while ((element != mAvailable.begin()) && (element->size > std::prev(element)->size))
		{
			std::swap(element->adress, std::prev(element)->adress);
			std::swap(element->size, std::prev(element)->size);
			element--;
		}
	}
	~MemoryPool();

public:
	char* startAdress;
	std::list<PoolElement> mAvailable;
	std::set<PoolElement> mAllocated;
	std::size_t poolSize;
};
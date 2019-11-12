#pragma once
#include "stdafx.h"

class PoolElement
{
public:
	PoolElement(char* adress, size_t size) : adress(adress), size(size) {}
	void updateElement(char* adress, size_t size)
	{
		this->adress = adress;
		this->size = size;
	}

public:
	char* adress;
	size_t size;
};


class MemoryPool
{
public:
	MemoryPool(size_t poolSize);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	void __cdecl freeMemory(void* aBlock, int aBlockUse);

	~MemoryPool();

public:
	void* startAdress;
	std::list<PoolElement> mAvailable;
	std::list<PoolElement> mAllocated;
	std::size_t poolSize;
};
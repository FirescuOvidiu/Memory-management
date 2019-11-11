#pragma once
#include "stdafx.h"


class MemoryPool
{
public:
	MemoryPool(size_t poolSize);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber);
	void __cdecl freeMemory(void* aBlock, int aBlockUse);

	~MemoryPool();

public:
	std::list<char*> mAvailable;
	std::list<char*> mAllocated;
	std::size_t poolSize;
};
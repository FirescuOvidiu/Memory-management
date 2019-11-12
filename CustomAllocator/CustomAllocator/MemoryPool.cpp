#include "stdafx.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t poolSize) : poolSize(poolSize)
{
	mAvailable.push_back(PoolElement(new char[poolSize], poolSize));
	startAdress = static_cast<void*>(mAvailable.front().adress);
}


void* __cdecl MemoryPool::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	if ((mAvailable.empty()) || (aSize > mAvailable.front().size))
	{
		// Bad alloc
	}

	PoolElement curr = mAvailable.front();
	void* block = static_cast<void*>(mAvailable.front().adress);

	mAllocated.push_back(PoolElement(curr.adress, aSize));
	mAvailable.front().updateElement(curr.adress + aSize, curr.size - aSize);

	return block;
}

void __cdecl MemoryPool::freeMemory(void* aBlock, int aBlockUse)
{
	return;
}

MemoryPool::~MemoryPool()
{

}
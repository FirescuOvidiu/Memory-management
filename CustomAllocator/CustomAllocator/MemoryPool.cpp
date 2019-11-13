#include "stdafx.h"


MemoryPool::MemoryPool(size_t poolSize) : poolSize(poolSize)
{
	mAvailable.push_back(PoolElement(new char[poolSize], poolSize));
	startAdress = mAvailable.front().adress;
}


void* __cdecl MemoryPool::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	if ((mAvailable.empty()) || (aSize > mAvailable.front().size))
	{
		// Bad alloc
	}

	auto currBlock = mAvailable.begin();
	void* block = static_cast<void*>(currBlock->adress);

	mAllocated.insert(PoolElement(currBlock->adress, aSize));
	mAvailable.front().updateElement(currBlock->adress + aSize, currBlock->size - aSize);
	while ((std::next(currBlock) != mAvailable.end()) && (currBlock->adress > std::next(currBlock)->adress))
	{
		std::swap(currBlock->adress, std::next(currBlock)->adress);
		std::swap(currBlock->size, std::next(currBlock)->size);
		currBlock++;
	}

	return block;
}

void __cdecl MemoryPool::freeMemory(void* aBlock, int aBlockUse)
{
	return;
}

MemoryPool::~MemoryPool()
{

}
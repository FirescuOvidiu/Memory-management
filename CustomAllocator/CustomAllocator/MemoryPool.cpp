#include "stdafx.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t poolSize) : poolSize(poolSize)
{

}

void* __cdecl MemoryPool::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	return nullptr;
}

void __cdecl MemoryPool::freeMemory(void* aBlock, int aBlockUse)
{
	return;
}

MemoryPool::~MemoryPool()
{

}

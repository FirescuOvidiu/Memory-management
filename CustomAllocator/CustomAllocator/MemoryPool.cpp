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

	std::cout << "\n\n";

	for (auto it = mAvailable.begin(); it != mAvailable.end(); it++)
	{
		std::cout << static_cast<void*> (it->adress) << " " << it->size << "\t";
	}
	std::cout << "\n";
	for (auto it = mAllocated.begin(); it != mAllocated.end(); it++)
	{
		std::cout << static_cast<void*> (it->adress) << " " << it->size << "\t";
	}

	return block;
}

void __cdecl MemoryPool::freeMemory(void* aBlock, int aBlockUse)
{
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (it == mAllocated.end())
	{
		// Invalid adress
		return;
	}

	PoolElement deletedMemory = *it;

	mAllocated.erase(it);

	std::list<PoolElement>::iterator compareSize = mAvailable.begin();
	std::list<PoolElement>::iterator left = mAvailable.end();
	std::list<PoolElement>::iterator right = mAvailable.end();

	for (std::list<PoolElement>::iterator currElement = mAvailable.begin(); currElement != mAvailable.end(); currElement++)
	{
		if (compareSize->size > deletedMemory.size)
		{
			compareSize++;
		}

		if (currElement->adress + currElement->size == deletedMemory.adress)
		{
			left = currElement;
		}

		if (deletedMemory.adress + deletedMemory.size == currElement->adress)
		{
			right = currElement;
		}
	}

	if ((left == mAvailable.end()) && (right == mAvailable.end()))
	{
		mAvailable.insert(compareSize, deletedMemory);
		std::cout << "\n\n";

		for (auto it = mAvailable.begin(); it != mAvailable.end(); it++)
		{
			std::cout << static_cast<void*> (it->adress) << " " << it->size << "\t";
		}
		std::cout << "\n";
		for (auto it = mAllocated.begin(); it != mAllocated.end(); it++)
		{
			std::cout << static_cast<void*> (it->adress) << " " << it->size << "\t";
		}
		return;
	}

	if ((left != mAvailable.end()) && (right != mAvailable.end()))
	{
		left->size = left->size + right->size + deletedMemory.size;
		mAvailable.erase(right);

		MemoryPool::maintainSorted(left);
	}
	else
	{
		if (left != mAvailable.end())
		{
			left->size = left->size + deletedMemory.size;

			MemoryPool::maintainSorted(left);
		}

		if (right != mAvailable.end())
		{
			right->adress = deletedMemory.adress;
			right->size = right->size + deletedMemory.size;

			MemoryPool::maintainSorted(right);
		}
	}

	std::cout << "\n\n";

	for (auto it = mAvailable.begin(); it != mAvailable.end(); it++)
	{
		std::cout << static_cast<void*> (it->adress) << " " << it->size << "\t";
	}
	std::cout << "\n";
	for (auto it = mAllocated.begin(); it != mAllocated.end(); it++)
	{
		std::cout << static_cast<void*> (it->adress) << " " << it->size << "\t";
	}
}

MemoryPool::~MemoryPool()
{
	if (!mAllocated.empty())
	{
		// Memory leaks
	}

	delete[] startAdress;
}
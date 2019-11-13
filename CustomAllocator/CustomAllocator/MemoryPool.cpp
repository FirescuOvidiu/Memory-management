#include "stdafx.h"

/*
	Constructor used to allocate the memory pool and 
	save the first adress for deallocation after the TU is finished in case of memory leaks
 */
MemoryPool::MemoryPool(size_t poolSize) : poolSize(poolSize)
{
	mAvailable.push_back(PoolElement(new char[poolSize], poolSize));
	startAdress = mAvailable.front().adress;
}


/*
	Function used to allocate memory for the user
	Returns an adress to an open block in our memory pool
*/
void* __cdecl MemoryPool::allocMemory(size_t aSize, int aBlockUse, char const* aFileName, int aLineNumber)
{
	// If we don't have enough memory available or the biggest contiguous memory is smaller than the memory request 
	// Then we will throw an exception
	if ((mAvailable.empty()) || (aSize > mAvailable.front().size))
	{
		// Bad alloc
	}

	// We will save in block the adress of the memory that will be given for the user to use
	auto currBlock = mAvailable.begin();
	void* block = static_cast<void*>(currBlock->adress);

	// Insert the adress and the size of the block in the memory allocated
	mAllocated.insert(PoolElement(currBlock->adress, aSize));

	// Update the block with the new adress and size and mantain the list sorted 
	mAvailable.front().updateElement(currBlock->adress + aSize, currBlock->size - aSize);
	while ((std::next(currBlock) != mAvailable.end()) && (currBlock->adress > std::next(currBlock)->adress))
	{
		std::swap(currBlock->adress, std::next(currBlock)->adress);
		std::swap(currBlock->size, std::next(currBlock)->size);
		currBlock++;
	}

	return block;
}

/*
	Function used to deallocate memory
	- deletes the adress from the allocated block (mAllocated)
	- inserts the adress into the unallocated list (mAvailable)
*/
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

	// We parse the list with the available blocks and do 2 things
	// - check if the deleted block can be merged with another block of memory from the available memory (we have 3 cases here)
	// - memorate the position where the block will fit if it doesn't need to merge with other blocks
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

	// If it doesn't merge with other blocks (to make a bigger contiguous memory block)
	// we simply insert it so that the lists is mantained sorted
	if ((left == mAvailable.end()) && (right == mAvailable.end()))
	{
		mAvailable.insert(compareSize, deletedMemory);
		return;
	}

	// Merge the deleted block with a left block and a right block making
	// The right block is deleted and only the left block will remain with the size of all 3 blocks
	if ((left != mAvailable.end()) && (right != mAvailable.end()))
	{
		left->size = left->size + right->size + deletedMemory.size;
		mAvailable.erase(right);

		MemoryPool::maintainSorted(left);
	}
	else
	{
		// Merge the deleted block with the a left block
		if (left != mAvailable.end())
		{
			left->size = left->size + deletedMemory.size;

			MemoryPool::maintainSorted(left);
		}

		// Merge the deleted block with a right block
		if (right != mAvailable.end())
		{
			right->adress = deletedMemory.adress;
			right->size = right->size + deletedMemory.size;

			MemoryPool::maintainSorted(right);
		}
	}
}


/*
	Function used to maintain the list sorted descending by size
	Having the list sorted except one element that has the size bigger than the previous elements
	we try to find its position so that the list is sorted descending by size
*/
void MemoryPool::maintainSorted(std::list<PoolElement>::iterator& element)
{
	while ((element != mAvailable.begin()) && (element->size > std::prev(element)->size))
	{
		std::swap(element->adress, std::prev(element)->adress);
		std::swap(element->size, std::prev(element)->size);
		element--;
	}
}


/*
	Destructor used to deallocated the memory pool, allocated at the start of the program
*/
MemoryPool::~MemoryPool()
{
	// If we still have elements allocated that means the program has memory leaks
	if (!mAllocated.empty())
	{
		// Memory leaks
	}

	delete[] startAdress;
}
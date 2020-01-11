#include "stdafx.h"

/*
	Constructor used to allocate the memory pool, initialize data members and
	save the first address for deallocation after the T.U. is finished in case of memory leaks
 */
WorstFit::WorstFit(size_t poolSize) : poolSize(poolSize)
{
	startAddress = new char[poolSize];
	mAvailable.push_back(PoolElement(startAddress, poolSize));

	// Updating the log with informations about size of the memory pool, total memory available, start address
	diagTools.updateInfoLog(poolSize, startAddress);

	// Initialize data members of the diagnostics
	diagTools.initDiagnostics((int)poolSize);

	// Initialize data members of the external diagnostics
	diagTools.initExternalFrag((int)poolSize);
}


/*
	Function used to allocate memory for the user
	Returns an address to an open block of memory of size aSize in our memory pool
*/
void* __cdecl WorstFit::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	// Thorwing exception in case we can't allocate the memory because different reasons (see function)
	if (checkBadAlloc(aSize))
	{
		std::bad_alloc exception;
		throw exception;
	}

	auto currBlock = mAvailable.begin();
	void* block = static_cast<void*>(currBlock->address);

	// Insert the address and the size of the block in the memory allocated
	mAllocated.insert(PoolElement(currBlock->address, aSize));

	// Update the block with the new address and size and mantain the list sorted 
	mAvailable.front().updateElement(currBlock->address + aSize, currBlock->size - aSize);
	while ((std::next(currBlock) != mAvailable.end()) && (currBlock->size < std::next(currBlock)->size))
	{
		std::swap(currBlock->address, std::next(currBlock)->address);
		std::swap(currBlock->size, std::next(currBlock)->size);
		currBlock++;
	}

	// Updating the diagnostics
	diagTools.updateDiagnostics((int)aSize);

	// Update the external disagnostics
	diagTools.updateExternalFrag((int)mAvailable.front().size);

	// Update log
	diagTools.increaseAllocOrDealloc(-(int)aSize);

	return block;
}


/*
	Function used to deallocate memory. Does two things:
	- removes the address from the mAllocated (mAllocated contains the blocks allocated)
	- inserts the address into the mAvailable (mAvailable contains the blocks unallocated)
*/
void __cdecl WorstFit::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	// Searching the address that the user wants to delete in mAllocated
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, it))
	{
		std::abort();
	}

	PoolElement deallocatedMemory = *it;

	// Remove the adress from the allocated block
	mAllocated.erase(it);

	// Insert the address into the unallocated list (mAvailable)
	insertIntoAvailableMemory(deallocatedMemory);

	// Updating the diagnostics
	diagTools.updateDiagnostics(-1);

	// Update the external disagnostics
	diagTools.updateExternalFrag((int)mAvailable.front().size);

	// Update log
	diagTools.increaseAllocOrDealloc((int)deallocatedMemory.size);
}


/*
	Checking if we can't allocate memory for the user because different reasons
*/
bool WorstFit::checkBadAlloc(size_t aSize)
{
	// If we don't have enough memory available or 
	// The biggest contiguous memory is smaller than the memory requested
	if (mAvailable.empty()|| (aSize > mAvailable.front().size))
	{
		// Update log
		diagTools.updateErrorLog(0, aSize, mAvailable.front().size, "Bad alloc");

		return true;
	}

	return false;
}


/*
	This function is used to check if the user tried to deallocate a block of memory which is not allocated
*/
bool WorstFit::checkInvalidAddress(void* aBlock,const std::set<PoolElement>::iterator& it)
{
	if (it == mAllocated.end())
	{
		// Update log
		diagTools.updateErrorLog(aBlock, 0, 0, "Invalid Address");

		return true;
	}

	return false;
}


/*
	Function used to check if the program has memory leaks
*/
void WorstFit::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		// Update log
		diagTools.updateWarningLog(poolSize);
	}
}


/*
	Function used to maintain the list sorted descending by size
	We have the list sorted except one element that has the size bigger than the previous elements
	we try to find its position so that the list is maintained sorted descending by size
*/
void WorstFit::maintainListSorted(std::list<PoolElement>::iterator& element)
{
	while ((element != mAvailable.begin()) && (element->size > std::prev(element)->size))
	{
		std::swap(element->address, std::prev(element)->address);
		std::swap(element->size, std::prev(element)->size);
		element--;
	}
}


/*
	Insert the address that was removed from the allocated block into the unallocated list (mAvailable)
*/
void WorstFit::insertIntoAvailableMemory(const PoolElement& deletedMemory)
{

	std::list<PoolElement>::iterator newPosition = mAvailable.begin();
	std::list<PoolElement>::iterator leftBlock = mAvailable.end(), rightBlock = mAvailable.end();

	// We parse the list with the available blocks and do two things
	// - check if the deleted block can be merged with another block of memory from the available memory (we have 3 cases here)
	// - memorate the position where the block will fit if it doesn't need to merge with other blocks
	for (std::list<PoolElement>::iterator currElement = mAvailable.begin(); currElement != mAvailable.end(); currElement++)
	{
		if (newPosition->size > deletedMemory.size)
		{
			newPosition++;
		}

		if (currElement->address + currElement->size == deletedMemory.address)
		{
			leftBlock = currElement;
		}

		if (deletedMemory.address + deletedMemory.size == currElement->address)
		{
			rightBlock = currElement;
		}
	}

	// If it doesn't merge with other blocks (to make a bigger continuous memory block)
	// we simply insert it so that the lists is mantained sorted
	if ((leftBlock == mAvailable.end()) && (rightBlock == mAvailable.end()))
	{
		mAvailable.insert(newPosition, deletedMemory);
	}
	else
	{
		// Merge the deleted block with a left block and a right block
		// The right block is deleted and only the left block will remain with the size of all 3 blocks
		if ((leftBlock != mAvailable.end()) && (rightBlock != mAvailable.end()))
		{
			leftBlock->size = leftBlock->size + rightBlock->size + deletedMemory.size;
			mAvailable.erase(rightBlock);

			WorstFit::maintainListSorted(leftBlock);
		}
		else
		{
			// Merge the deleted block with the a left block
			if (leftBlock != mAvailable.end())
			{
				leftBlock->size = leftBlock->size + deletedMemory.size;

				WorstFit::maintainListSorted(leftBlock);
			}

			// Merge the deleted block with a right block
			if (rightBlock != mAvailable.end())
			{
				rightBlock->address = deletedMemory.address;
				rightBlock->size = rightBlock->size + deletedMemory.size;

				WorstFit::maintainListSorted(rightBlock);
			}
		}
	}
}


/*
	Destructor used to deallocated the memory pool, allocated at the start of the program
	Also we check if the program has memory leaks
*/
WorstFit::~WorstFit()
{
	checkMemoryLeaks();

	delete[] startAddress;
}
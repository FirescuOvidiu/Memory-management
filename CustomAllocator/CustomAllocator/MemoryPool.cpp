#include "stdafx.h"

/*
	Constructor used to allocate the memory pool and
	save the first address for deallocation after the T.U. is finished in case of memory leaks
 */
MemoryPool::MemoryPool(size_t poolSize) : poolSize(poolSize)
{
	mAvailable.push_back(PoolElement(new char[poolSize], poolSize));
	startAddress = mAvailable.front().address;

	// Updating the log with informations about size of the memory pool, total memory available, start address
	log.updateLog("Size of the memory pool: " + std::to_string(poolSize) + " bytes.", LogLevel::Log_Level_Info);
	log.totalMemoryAvailable = (int)poolSize;
	std::stringstream ss;
	ss << static_cast<void*>(startAddress);
	log.updateLog("Start address: " + ss.str(), LogLevel::Log_Level_Info);

	// Initialize data members of the diagnostics
	diag.initializeDiagnostics((int)poolSize);
}


/*
	Function used to allocate memory for the user
	Returns an address to an open block of memory of size aSize in our memory pool
*/
void* __cdecl MemoryPool::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	// Updating the log with informations about memory available before allocation, size of the memory need to be allocated
	log.updateDebugLog("Memory available before allocation: " + std::to_string(log.totalMemoryAvailable) + ". Memory need to allocate: " + std::to_string(aSize), mAvailable, mAllocated);
	log.increaseAllocations((int)aSize);

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

	// Updating the log with information about memory available after allocation
	log.updateDebugLog("Memory Available after allocation: " + std::to_string(mAvailable.front().size), mAvailable, mAllocated);

	// Updating the diagnostics
	diag.updateMemoryInf(diag.getTotalMemory() - log.totalMemoryAvailable, (int)(mAvailable.front().size));
	diag.updateSegmentInf(mAvailable);

	return block;
}


/*
	Function used to deallocate memory. Does two things:
	- removes the address from the mAllocated (mAllocated contains the blocks allocated)
	- inserts the address into the mAvailable (mAvailable contains the blocks unallocated)
*/
void __cdecl MemoryPool::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	// Searching the address that the user wants to delete in mAllocated
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, it))
	{
		return;
	}

	// Updating the log with informations about memory available before deallocation and the size of memory that needs to be deallocate
	log.updateDebugLog("Memory available before deallocation: " + std::to_string(log.totalMemoryAvailable) + ". Memory to deallocate: " + std::to_string((*it).size), mAvailable, mAllocated);
	log.increaseDeallocations((int)it->size);

	PoolElement deletedMemory = *it;

	// Remove the adress from the allocated block
	mAllocated.erase(it);

	// Insert the address into the unallocated list (mAvailable)
	insertIntoAvailableMemory(deletedMemory);

	// Updating the log with informations about the memory available after deallocation
	log.updateDebugLog("Memory available after deallocation: " + std::to_string(log.totalMemoryAvailable), mAvailable, mAllocated);

	// Updating the diagnostics
	diag.updateMemoryInf(diag.getTotalMemory() - log.totalMemoryAvailable, (int)(mAvailable.front().size));
}


/*
	Checking if we can't allocate memory for the user because different reasons
*/
bool MemoryPool::checkBadAlloc(size_t aSize)
{
	// If we don't have enough memory available
	if (mAvailable.empty())
	{
		// Updating the log
		log.updateLog("Bad alloc because we don't have enough memory available", LogLevel::Log_Level_Error);
		log.~Logger();

		return true;
	}

	//  If the biggest contiguous memory is smaller than the memory request 
	if (aSize > mAvailable.front().size)
	{
		// Updating the log
		log.updateLog("Bad alloc because the biggest continuous memory is smaller than the memory request.", LogLevel::Log_Level_Error);
		log.updateLog("Biggest continuous memory: " + std::to_string(mAvailable.front().size), LogLevel::Log_Level_Error);
		log.updateLog("Memory needed : " + std::to_string(aSize), LogLevel::Log_Level_Error);
		log.~Logger();

		return true;
	}

	return false;
}


/*
	This function is used to check if the user tried to deallocate a block of memory which is not allocated
*/
bool MemoryPool::checkInvalidAddress(void* aBlock, std::set<PoolElement>::iterator it)
{
	if (it == mAllocated.end())
	{
		std::stringstream ss;
		ss << aBlock;

		// Updating the log
		log.updateLog("The application tries to deallocate an address that's not allocated. Adress: " + ss.str() + "\n", LogLevel::Log_Level_Warning);

		return true;
	}

	return false;
}


/*
	Function used to check if the program has memory leaks
*/
void MemoryPool::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		// Updating the log if the program has memory leaks
		log.updateLog("The application has memory leaks !!", LogLevel::Log_Level_Warning);
		log.updateLog("The size of the memory allocated that wasn't deallocated: " + std::to_string((int)poolSize - log.totalMemoryAvailable) + " bytes.", LogLevel::Log_Level_Warning);
	}
}


/*
	Function used to maintain the list sorted descending by size
	We have the list sorted except one element that has the size bigger than the previous elements
	we try to find its position so that the list is maintained sorted descending by size
*/
void MemoryPool::maintainSorted(std::list<PoolElement>::iterator& element)
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
void MemoryPool::insertIntoAvailableMemory(const PoolElement& deletedMemory)
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

			MemoryPool::maintainSorted(leftBlock);
		}
		else
		{
			// Merge the deleted block with the a left block
			if (leftBlock != mAvailable.end())
			{
				leftBlock->size = leftBlock->size + deletedMemory.size;

				MemoryPool::maintainSorted(leftBlock);
			}

			// Merge the deleted block with a right block
			if (rightBlock != mAvailable.end())
			{
				rightBlock->address = deletedMemory.address;
				rightBlock->size = rightBlock->size + deletedMemory.size;

				MemoryPool::maintainSorted(rightBlock);
			}
		}
	}
}


/*
	Destructor used to deallocated the memory pool, allocated at the start of the program
	Also we check if the program has memory leaks
*/
MemoryPool::~MemoryPool()
{
	checkMemoryLeaks();

	delete[] startAddress;
}
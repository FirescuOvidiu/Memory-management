#include "stdafx.h"


/*
	Constructor used to allocate the memory pool, initialize data members and
	save the start address to deallocate the memory pool and check for memory leaks after the T.U. is finished
 */
BestFit::BestFit(size_t poolSize) : poolSize(poolSize)
{
	startAddress = new char[poolSize];
	mAvailable.push_back(PoolElement(startAddress, poolSize));
	PoolElement::setStartAddress(startAddress);

	// Initialize data members of logger
	log.initLogger(poolSize);
}


std::ostream& BestFit::write(std::ostream& output) const
{
	// TODO: insert return statement here
	return output;
}


std::istream& BestFit::read(std::istream& input)
{
	// TODO: insert return statement here
	return input;
}


/*
	Function used to allocate memory for the user
	Returns an address to an open block of memory of size aSize in our memory pool
*/
void* __cdecl BestFit::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	std::list<PoolElement>::iterator currBlockAvailable = mAvailable.begin();

	while (currBlockAvailable != mAvailable.end() && currBlockAvailable->size - aSize <= 0)
	{
		currBlockAvailable++;
	}
	
	if (checkBadAlloc(aSize,currBlockAvailable))
	{
		std::bad_alloc exception;
		throw exception;
	}

	for (auto nextBlockAvailable = std::next(currBlockAvailable, 1); nextBlockAvailable != mAvailable.end(); nextBlockAvailable++)
	{
		if (currBlockAvailable->size - aSize <= nextBlockAvailable->size - aSize)
		{
			currBlockAvailable = nextBlockAvailable;
		}
	}

	void* block = static_cast<void*>(currBlockAvailable->address);

	// Insert the address and the size of the block in the memory allocated
	mAllocated.insert(PoolElement(currBlockAvailable->address, aSize));

	currBlockAvailable->updateElement(currBlockAvailable->address + aSize, currBlockAvailable->size - aSize);
	if (currBlockAvailable->size == 0)
	{
		mAvailable.erase(currBlockAvailable);
	}

	// Update logger
	log.increaseAllocOrDealloc(-(int)aSize);

	return block;
}


/*
	Function used to deallocate memory. Does two things:
	- removes the address from the mAllocated (mAllocated contains the blocks allocated)
	- inserts the address into the mAvailable (mAvailable contains the blocks unallocated)
*/
void __cdecl BestFit::freeMemory(void* aBlock, int)
{
	// Search the address that the user wants to delete in mAllocated
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

	// Update Logger
	log.increaseAllocOrDealloc((int)deallocatedMemory.size);
}


/*
	The method returns a pair with biggest contiguous memory and
	total memory available used to evaluate the fragmentation state of the custom allocator
*/
std::pair<int, int> BestFit::getCurrentState() const
{
	int biggestContMemory = 0, memoryAvailable = 0, memoryAllocated = 0;

	for (const auto& itMemoryAllocated : mAllocated)
	{
		memoryAllocated += (int)itMemoryAllocated.size;
	}

	memoryAvailable = (int)poolSize - memoryAllocated;
	for (const auto& currBlockAvailable : mAvailable)
	{
		if (biggestContMemory < currBlockAvailable.size)
		{
			biggestContMemory = currBlockAvailable.size;
		}
	}

	return std::make_pair(biggestContMemory, memoryAvailable);
}


/*
	Methods used to store into a file the state of the memory file
	The file will be imported into excel and make a chart with the information present
*/
void BestFit::showCurrentState() const
{
	std::ofstream output("memoryState.txt", std::ofstream::out);
	std::list<PoolElement> auxMAvailable = mAvailable;

	// Sorting the list of memory available by address
	auxMAvailable.sort();

	std::list<PoolElement>::const_iterator currAvailableBlock = auxMAvailable.cbegin();
	std::set<PoolElement>::const_iterator currAllocatedBlock = mAllocated.cbegin();

	// We have two structures sorted by address and we need to write 
	// into a file the elements from the both structures sorted by address
	// We parse the structures simultaneous and compare the elements
	while (currAvailableBlock != auxMAvailable.cend() && currAllocatedBlock != mAllocated.cend())
	{
		if (currAvailableBlock->address - startAddress < currAllocatedBlock->address - startAddress)
		{
			output << currAvailableBlock->size << "\n";
			output << currAllocatedBlock->size << "\n";
		}
		else
		{
			output << currAllocatedBlock->size << "\n";
			output << currAvailableBlock->size << "\n";
		}

		currAvailableBlock++;
		currAllocatedBlock++;
	}

	if (currAvailableBlock != auxMAvailable.cend())
	{
		output << currAvailableBlock->size << "\n";
	}
	if (currAllocatedBlock != mAllocated.cend())
	{
		output << currAllocatedBlock->size << "\n";
	}

	output.close();
}


/*
	Destructor used to deallocated the memory pool allocated at the start of the program
	Also we check if the program has memory leaks
*/
BestFit::~BestFit()
{
	checkMemoryLeaks();

	delete[] startAddress;
}


/*
	Check if we can't allocate memory for the user because different reasons
*/
bool BestFit::checkBadAlloc(size_t aSize, std::list<PoolElement>::iterator& currBlockAvailable)
{
	// If we don't have enough memory available or 
	// The biggest contiguous memory is smaller than the memory requested
	if (currBlockAvailable == mAvailable.end())
	{
		int biggestContiguousMemory = 0;

		// Find biggest contiguous memory available
		for (const auto& blockAvailable : mAvailable)
		{
			if (biggestContiguousMemory < blockAvailable.size)
			{
				biggestContiguousMemory = blockAvailable.size;
			}
		}

		// Update log
		log.updateErrorLog(0, aSize, biggestContiguousMemory, "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


/*
	Method used to check if the user tried to deallocate a block of memory which is not allocated
*/
bool BestFit::checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& it)
{
	if (it == mAllocated.end())
	{
		// Update log
		log.updateErrorLog(aBlock, 0, 0, "Invalid Address");
		log.~Logger();

		return true;
	}

	return false;
}


/*
	Methods used to check if the application has memory leaks
*/
void BestFit::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		// Update log
		log.updateWarningLog();
	}
}


/*
	Insert the block of memory that was removed from the allocated set (mAllocated) into the unallocated list (mAvailable)
*/
void BestFit::insertIntoAvailableMemory(const PoolElement& deallocatedMemory)
{
	// leftBlock will memorate the left block with which the deallocated block will merge
	// rightBlock will memorate the right block with which the deallocated block will merge
	std::list<PoolElement>::iterator leftBlock = mAvailable.end(), rightBlock = mAvailable.end();

	// We parse the list with the available blocks and check if the deallocated block 
	// can be merged with another block of memory from the available memory
	for (std::list<PoolElement>::iterator currElement = mAvailable.begin(); currElement != mAvailable.end(); currElement++)
	{
		if (currElement->address + currElement->size == deallocatedMemory.address)
		{
			leftBlock = currElement;
		}

		if (deallocatedMemory.address + deallocatedMemory.size == currElement->address)
		{
			rightBlock = currElement;
		}
	}

	// If it doesn't merge with other blocks (to make a bigger continuous memory block)
	// we simply insert it at the end of list
	if ((leftBlock == mAvailable.end()) && (rightBlock == mAvailable.end()))
	{
		mAvailable.push_back(deallocatedMemory);
	}
	else
	{
		// Merge the deallocated block with a left block and a right block
		// The right block is deleted and only the left block will remain with the size equal to sum of all 3 blocks
		if ((leftBlock != mAvailable.end()) && (rightBlock != mAvailable.end()))
		{
			leftBlock->size = leftBlock->size + rightBlock->size + deallocatedMemory.size;
			mAvailable.erase(rightBlock);
		}
		else
		{
			// Merge the deallocated block with the a left block
			if (leftBlock != mAvailable.end())
			{
				leftBlock->size = leftBlock->size + deallocatedMemory.size;
			}

			// Merge the deallocated block with a right block
			if (rightBlock != mAvailable.end())
			{
				rightBlock->address = deallocatedMemory.address;
				rightBlock->size = rightBlock->size + deallocatedMemory.size;
			}
		}
	}
}
#include "stdafx.h"


/*
	Constructor used to allocate the memory pool, initialize data members and
	save the first address for deallocation after the T.U. is finished in case of memory leaks
 */
BuddySystem::BuddySystem(size_t poolSize)
{
	// this->poolSize will be the first number that is a power of 2 and is smaller or equal with poolSize
	// If poolSize is a power of 2 then this->poolSize will be equal with poolSize
	this->poolSize = (int)pow(2, (int)floor(log2((int)poolSize)));
	startAddress = new char[this->poolSize];

	mAvailable.resize((int)(log2(this->poolSize) + 1));
	mAvailable[mAvailable.size() - 1].insert(PoolElement(startAddress, this->poolSize));

	// Initialize data members of logger
	log.initLogger(poolSize, startAddress);
}


/*
	Function used to allocate memory
	Returns an address to an open block of memory of size:
	first number that is a power of 2 and is bigger or equal with the aSize
*/
void* __cdecl BuddySystem::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	// Position will be used to find an open block of memory 
	int position = (int)std::ceil(log2(aSize));

	// We parse memory available and search for a block of memory that is 
	// bigger or equal to aSize
	while ((position < mAvailable.size()) && (mAvailable[position].empty()))
	{
		position++;
	}

	// Throwing exception in case we can't allocate the memory because different reasons (see function)
	if (checkBadAlloc(aSize, position))
	{
		std::bad_alloc exception;
		throw exception;
	}

	PoolElement availableBlock = getAvailableBlock(aSize, position);

	// Update logger
	log.increaseAllocOrDealloc(-(int)availableBlock.size);

	availableBlock.size = aSize;
	mAllocated.insert(availableBlock);

	return availableBlock.address;
}


void __cdecl BuddySystem::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	// Search the address that the user wants to delete in mAllocated
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, it))
	{
		std::abort();
	}

	int deallocatedSize = (int)pow(2, (int)std::ceil(log2((*it).size)));
	PoolElement deallocatedMemory = *it;

	// Remove the adress from the allocated block
	mAllocated.erase(it);

	// Insert the address into the available memory (mAvailable)
	deallocatedMemory.size = deallocatedSize;
	insertIntoAvailableMemory(deallocatedMemory);

	// Update logger
	log.increaseAllocOrDealloc((int)deallocatedMemory.size);
}


/*
	Method used to get the memory allocated and memory requested by the user at this point
	This variables are used to evaluate the fragmentation state of the custom allocator
*/
std::pair<int, int> BuddySystem::getCurrentState() const
{
	int memoryAllocated = 0, memoryAvailable = 0, memoryRequested = 0;

	for (const auto& itMemoryAllocated : mAllocated)
	{
		memoryRequested += (int)itMemoryAllocated.size;
	}

	for (const auto& itMemoryAvailable : mAvailable)
	{
		for (const auto& currAvailableBlock : itMemoryAvailable)
		{
			memoryAvailable += (int)currAvailableBlock.size;
		}
	}

	memoryAllocated = (int)poolSize - memoryAvailable;

	return std::make_pair(memoryAllocated, memoryRequested);
}


//void BuddySystem::serialization(std::ofstream& output)
//{
//
//}
//
//
//void BuddySystem::deserialization(std::ifstream& input)
//{
//
//}


/*
	Check if we can't allocate memory for the user because different reasons
*/
bool BuddySystem::checkBadAlloc(size_t aSize, int& position)
{
	// If we don't have enough memory available or 
	// The biggest contiguous memory is smaller than the memory requested
	if (position >= mAvailable.size())
	{
		// Find the biggest continuous memory available
		position = (int)std::ceil(log2(aSize));
		if (position >= mAvailable.size())
		{
			position = (int)mAvailable.size() - 1;
		}
		while ((position >= 0) && (mAvailable[position].empty()))
		{
			position--;
		}

		// Update log
		log.updateErrorLog(0, aSize, (int)pow(2, position), "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


/*
	This function is used to check if the user tried to deallocate a block of memory which is not allocated
*/
bool BuddySystem::checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& it)
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
	Function used to check if the program has memory leaks
*/
void BuddySystem::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		// Update log
		log.updateWarningLog(poolSize);
	}
}


/*
	Method used to find the open block that is of size:
	first number that is a power of 2 and is bigger or equal with the aSize
*/
PoolElement BuddySystem::getAvailableBlock(size_t aSize, int position)
{
	PoolElement firstNewElement, secondNewElement, currBlock;

	// Get the open block from the memory available
	currBlock = *mAvailable[position].begin();
	mAvailable[position].erase(mAvailable[position].begin());

	// While half of current open block is bigger than aSize then 
	// we split the current open block into two blocks
	position--;
	while (pow(2, position) >= aSize)
	{
		// Split the current open block into two blocks
		firstNewElement.updateElement(currBlock.address, currBlock.size / 2);
		secondNewElement.updateElement(currBlock.address + currBlock.size / 2, currBlock.size / 2);

		// The first half becomes the current block
		currBlock = firstNewElement;
		// The second half is inserted into the vector used to keep track of the available memory
		mAvailable[position].insert(secondNewElement);
		position--;
	}

	return currBlock;
}


/*
	Insert the address that was removed from the allocated block into the available memory (mAvailable)
*/
void BuddySystem::insertIntoAvailableMemory(PoolElement& deallocatedMemory)
{
	PoolElement adjacentBlock;
	int position = (int)std::ceil(log2(deallocatedMemory.size));

	// Find the adjacent block of the deallocated block 
	findAdjacentBlock(adjacentBlock, deallocatedMemory);

	// Check if the adjacent block is unallocated and available
	auto it = mAvailable[position].find(adjacentBlock);

	while (it != mAvailable[position].end())
	{
		// Merge deallocated block of memory with the adjacent block
		if ((*it).address < deallocatedMemory.address)
		{
			deallocatedMemory.address = (*it).address;
		}
		deallocatedMemory.size *= 2;

		mAvailable[position++].erase(it);

		// Find the new adjacent block of the two merged blocks
		findAdjacentBlock(adjacentBlock, deallocatedMemory);
		it = mAvailable[position].find(adjacentBlock);
	}

	mAvailable[position].insert(deallocatedMemory);
}


/*
	Method used to find the adjacent block of the deallocated block of memory
*/
void BuddySystem::findAdjacentBlock(PoolElement& buddy, const PoolElement& deallocatedMemory) const
{
	buddy.size = deallocatedMemory.size;
	if (((deallocatedMemory.address - startAddress) / deallocatedMemory.size) % 2)
	{
		buddy.address = deallocatedMemory.address - deallocatedMemory.size;
	}
	else
	{
		buddy.address = deallocatedMemory.address + deallocatedMemory.size;
	}
}


/*
	Destructor used to deallocated the memory pool, allocated at the start of the program
	Also we check if the program has memory leaks
*/
BuddySystem::~BuddySystem()
{
	checkMemoryLeaks();

	delete[] startAddress;
}
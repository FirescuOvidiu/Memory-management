#include "stdafx.h"


/*
	Constructor used to allocate the memory pool, initialize data members and
	save the first address for deallocation after the T.U. is finished in case of memory leaks
 */
BuddySystem::BuddySystem(size_t poolSize)
{
	// We find the first number that is a power of 2 and is smaller or equal with the aSize
	// If poolSize is a power of 2 then findPoolSize will be equal with poolSize
	int findPoolSize = 1;
	while (findPoolSize <= poolSize)
	{
		findPoolSize *= 2;
	}
	findPoolSize /= 2;

	this->poolSize = findPoolSize;
	startAddress = new char[this->poolSize];

	mAvailable.resize((int)(log2(this->poolSize) + 1));
	mAvailable[mAvailable.size() - 1].insert(PoolElement(startAddress, this->poolSize));

	// Updating the log with informations about size of the memory pool, total memory available, start address
	log.updateInfoLog(this->poolSize, startAddress);

	// Initialize data members of the diagnostics
	diag.initializeDiagnostics((int)this->poolSize);
}


/*
	Function used to allocate memory for the user
	Returns an address to an open block of memory of size:
	first number that is a power of 2 and is bigger or equal with the aSize
*/
void* __cdecl BuddySystem::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	log.increaseAllocations((int)aSize);

	// Position will be used to find an open block of memory 
	int position = 0;
	// Thorwing exception in case we can't allocate the memory because different reasons (see function)
	if (checkBadAlloc(aSize, position))
	{
		std::bad_alloc exception;
		throw exception;
	}

	PoolElement availableBlock = getAvailableBlock(aSize,position);

	mAllocated.insert(availableBlock);
	return availableBlock.address;
}


void __cdecl BuddySystem::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	// Searching the address that the user wants to delete in mAllocated
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, it))
	{
		std::abort();
	}

	log.increaseDeallocations((int)it->size);

	PoolElement deallocatedMemory = *it;

	// Remove the adress from the allocated block
	mAllocated.erase(it);

	// Insert the address into the available memory (mAvailable)
	insertIntoAvailableMemory(deallocatedMemory);
}


/*
	Checking if we can't allocate memory for the user because different reasons
*/
bool BuddySystem::checkBadAlloc(size_t aSize, int& position)
{
	// We parse memory available and search for a block of memory that is 
	// bigger or equal to aSize
	position = (int)std::ceil(log2(aSize));
	while ((position < mAvailable.size()) && (mAvailable[position].empty()))
	{
		position++;
	}

	// If we don't have enough memory available or 
	// The biggest contiguous memory is smaller than the memory requested
	if (position == mAvailable.size())
	{
		// Find the biggest continuous memory available
		position = (int)std::ceil(log2(aSize));
		while ((position >= 0) && (mAvailable[position].empty()))
		{
			position--;
		}
		// Update log
		 log.updateErrorLog(0, aSize, pow(2,position), "Bad alloc");

		log.~Logger();
		diag.~Diagnostics();
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
		diag.~Diagnostics();
		diagExternal.~DiagnoseExternalFragmentation();
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
		// Updating the log if the program has memory leaks
		log.updateLog("The application has memory leaks !!", LogLevel::Log_Level_Warning);
		log.updateLog("The size of the memory allocated that wasn't deallocated: " + std::to_string((int)poolSize - log.totalMemoryAvailable) + " bytes.", LogLevel::Log_Level_Warning);
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
	currBlock = (*mAvailable[position].begin());
	mAvailable[position].erase(mAvailable[position].begin());

	// While half of current open block is bigger than aSize then 
	// we split the current open block into two blocks
	position--;
	while (pow(2, position) >= aSize)
	{
		// Split the current open block into two blocks
		firstNewElement.updateElement(currBlock.address, currBlock.size / 2);
		secondNewElement.updateElement(currBlock.address + currBlock.size / 2, currBlock.size / 2);

		// The current block becomes the first half of the open block
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
	PoolElement buddy;
	int position = 0;
	bool find = true;

	position = (int)std::ceil(log2(deallocatedMemory.size));
	buddy.size = deallocatedMemory.size;
	if ((int)log2(deallocatedMemory.address - startAddress) % 2 != 0)
	{
		buddy.address = deallocatedMemory.address - deallocatedMemory.size;
	}
	else
	{
		buddy.address = deallocatedMemory.address + deallocatedMemory.size;
	}

	auto it = mAvailable[position].find(buddy);
	while (it != mAvailable[position].end())
	{
		if ((*it).address < deallocatedMemory.address)
		{
			deallocatedMemory.address = (*it).address;
		}
		deallocatedMemory.size *= 2;
		mAvailable[position].erase(it);
		position++;

		buddy.size = deallocatedMemory.size;
		if ((int)log2(deallocatedMemory.address - startAddress) % 2 != 0)
		{
			buddy.address = deallocatedMemory.address - deallocatedMemory.size;
		}
		else
		{
			buddy.address = deallocatedMemory.address + deallocatedMemory.size;
		}
		it = mAvailable[position].find(buddy);
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
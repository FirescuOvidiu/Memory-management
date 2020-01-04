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

	mAvailable.resize((int)log2(this->poolSize) + 1);
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

	PoolElement firstNewElement, secondNewElement, currBlock;

	// Get the open block from the memory available
	currBlock = (*mAvailable[position].begin());
	mAvailable[position].erase(mAvailable[position].begin());

	// While half of current open block is bigger than aSize then 
	// we split the current open block in two blocks
	position--;
	while (pow(2, position) >= aSize)
	{
		// Split the current open block in two blocks
		firstNewElement.updateElement(currBlock.address, currBlock.size / 2);
		secondNewElement.updateElement(currBlock.address + currBlock.size / 2, currBlock.size / 2);

		// The current block becomes the first half of the open blocke
		currBlock = firstNewElement;
		// The second half is inserted into the vector used to keep track of the available memory
		mAvailable[position].insert(secondNewElement);
		position--;
	}

	mAllocated.insert(currBlock);
	return currBlock.address;
}


void __cdecl BuddySystem::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));
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
	Destructor used to deallocated the memory pool, allocated at the start of the program
	Also we check if the program has memory leaks
*/
BuddySystem::~BuddySystem()
{
	//Need to check for memory leaks

	delete[] startAddress;
}
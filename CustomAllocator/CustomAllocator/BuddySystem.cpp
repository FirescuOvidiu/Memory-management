#include "stdafx.h"


/*
	Constructor used to allocate the memory pool, initialize data members and
	save the first address for deallocation after the T.U. is finished in case of memory leaks
 */
BuddySystem::BuddySystem(size_t poolSize)
{
	// In case the poolSize is not a power of 2 we find the closest number to the poolSize
	// that is a power of 2
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

	int position = 0;
	if (checkBadAlloc(aSize, position))
	{
		std::bad_alloc exception;
		throw exception;
	}

	PoolElement currBlock((*mAvailable[position].begin()).address, (*mAvailable[position].begin()).size);
	mAvailable[position].erase(mAvailable[position].begin());

	if (aSize != pow(2, position))
	{
		PoolElement firstNewElement, secondNewElement;

		while (pow(2, position - 1) >= aSize)
		{
			firstNewElement.updateElement(currBlock.address, currBlock.size / 2);
			secondNewElement.updateElement(currBlock.address + currBlock.size / 2, currBlock.size / 2);
			
			position--;
			mAvailable[position].insert(secondNewElement);
			currBlock = firstNewElement;
		}
	}

	mAllocated.insert(currBlock);
	return currBlock.address;
}


void __cdecl BuddySystem::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));
}


bool BuddySystem::checkBadAlloc(size_t aSize, int& position)
{
	position = (int)std::ceil(log2(aSize));
	while ((position < mAvailable.size()) && (mAvailable[position].empty()))
	{
		position++;
	}

	if (position == mAvailable.size())
	{
		// Update log
		// log.updateErrorLog(0, aSize, mAvailable.front().size, "Bad alloc");

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
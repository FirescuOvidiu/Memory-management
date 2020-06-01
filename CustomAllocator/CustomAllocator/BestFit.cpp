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
}


std::istream& BestFit::read(std::istream& input)
{
	// TODO: insert return statement here
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


void __cdecl BestFit::freeMemory(void* aBlock, int)
{

}


std::pair<int, int> BestFit::getCurrentState() const
{
	return std::pair<int, int>();
}


void BestFit::showCurrentState() const
{

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


bool BestFit::checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& it)
{
	return false;
}


void BestFit::checkMemoryLeaks()
{

}
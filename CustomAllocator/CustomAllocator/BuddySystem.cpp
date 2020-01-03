#include "stdafx.h"


BuddySystem::BuddySystem(size_t poolSize)
{
	int findPoolSize = 1;
	while (findPoolSize < poolSize)
	{
		findPoolSize *= 2;
	}
	findPoolSize /= 2;

	this->poolSize = findPoolSize;
	startAddress = new char[this->poolSize];

	mAvailable.resize((int)log2(this->poolSize));
	mAvailable[this->poolSize - 1].insert(PoolElement(startAddress, this->poolSize));

	// Updating the log with informations about size of the memory pool, total memory available, start address
	log.updateInfoLog(this->poolSize, startAddress);

	// Initialize data members of the diagnostics
	diag.initializeDiagnostics((int)this->poolSize);
}


void* __cdecl BuddySystem::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	log.increaseAllocations((int)aSize);

	int position = 0;
	if (checkBadAlloc(aSize, position))
	{
		std::bad_alloc exception;
		throw exception;
	}

	auto currBlock = mAvailable[position].begin();
	mAvailable[position].erase(mAvailable[position].begin());

	if (aSize == pow(2, position))
	{
		return (*currBlock).address;
	}
	else
	{

	}

	return nullptr;
}


void __cdecl BuddySystem::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));
}


bool BuddySystem::checkBadAlloc(size_t aSize, int& position)
{
	position = std::ceil(log2(aSize));
	while ((position < mAvailable.size()) && (mAvailable[position].empty()))
	{
		position++;
	}

	if (position == mAvailable.size())
	{
		// Update log
		log.updateErrorLog(0, aSize, mAvailable.front().size, "Bad alloc");

		log.~Logger();
		diag.~Diagnostics();
		return true;
	}
	
	return false;
}


BuddySystem::~BuddySystem()
{
	delete[] startAddress;
}
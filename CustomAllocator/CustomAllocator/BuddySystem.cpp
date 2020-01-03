#include "stdafx.h"


BuddySystem::BuddySystem(size_t poolSize)
{
	this->poolSize = (int) log2((double)poolSize);
	startAddress = new char[this->poolSize];

	mAvailable.resize(this->poolSize);
	mAvailable[this->poolSize - 1].insert(PoolElement(startAddress, this->poolSize));

	// Updating the log with informations about size of the memory pool, total memory available, start address
	log.updateInfoLog(this->poolSize, startAddress);

	// Initialize data members of the diagnostics
	diag.initializeDiagnostics((int)this->poolSize);
}


void* __cdecl BuddySystem::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	log.increaseAllocations((int)aSize);


	return nullptr;
}


void __cdecl BuddySystem::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));
}


BuddySystem::~BuddySystem()
{
	delete[] startAddress;
}
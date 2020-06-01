#include "stdafx.h"


/*
	Constructor used to allocate the memory pool, initialize data members and
	save the start address to deallocate the memory pool and check for memory leaks after the T.U. is finished
 */
BestFit::BestFit(size_t poolSize) : poolSize(poolSize)
{
	startAddress = new char[poolSize];
	mAvailable.push_back(PoolElement(startAddress, poolSize));

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


void* __cdecl BestFit::allocMemory(size_t aSize, int, char const*, int)
{
	return nullptr;
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


BestFit::~BestFit()
{

}


bool BestFit::checkBadAlloc(size_t aSize)
{
	return false;
}


bool BestFit::checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& it)
{
	return false;
}


void BestFit::checkMemoryLeaks()
{

}
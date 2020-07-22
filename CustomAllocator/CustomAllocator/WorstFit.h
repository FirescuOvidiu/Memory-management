#pragma once
#include "stdafx.h"


/*
	Class used to implement a strategy for the allocator that uses the Worst Fit algorithm to allocate and deallocate memory
*/
class WorstFit : public Strategy
{
public:
	WorstFit(size_t poolSize);

	std::ofstream& write(std::ofstream& output) const override;
	std::ifstream& read(std::ifstream& input) override;

	void* __cdecl allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/) override;
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/) override;

	std::pair<int, int> getCurrentState() const;
	void showCurrentState() const;

	~WorstFit();

private:
	bool checkBadAlloc(size_t aSize);
	bool checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& it);
	void checkMemoryLeaks();

	void setStartAddress();
	void maintainListSorted(std::list<PoolElement>::iterator& element);
	void insertIntoAvailableMemory(const PoolElement& deallocatedMemory);

private:
	char* startAddress;
	std::list<PoolElement> mAvailable;	// Stores the memory unallocated sorted descending by the size
	std::set<PoolElement> mAllocated;	// Stores the memory allocated sorted ascending by the address
	std::size_t poolSize;				// The size of the memory pool
};
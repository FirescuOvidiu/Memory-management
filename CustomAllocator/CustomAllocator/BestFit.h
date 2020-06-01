#pragma once
#include "stdafx.h"


class BestFit : public Strategy
{
public:
	BestFit(size_t poolSize);

	std::ostream& write(std::ostream& output) const override;
	std::istream& read(std::istream& input) override;

	void* __cdecl allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/) override;
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/) override;

	std::pair<int, int> getCurrentState() const;
	void showCurrentState() const;

	~BestFit();

private:
	bool checkBadAlloc(size_t aSize);
	bool checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& it);
	void checkMemoryLeaks();

private:
	char* startAddress;					// Start address of the memory pool
	std::list<PoolElement> mAvailable;	// Stores the memory unallocated sorted descending by the size
	std::set<PoolElement> mAllocated;	// Stores the memory allocated sorted by the address
	std::size_t poolSize;				// The size of the memory pool
};
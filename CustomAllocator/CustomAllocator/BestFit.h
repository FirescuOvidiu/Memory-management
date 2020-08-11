#pragma once
#include "stdafx.h"


class BestFit final : public Strategy
{
public:
	BestFit(const size_t poolSize, const LogType logType);

	std::ofstream& write(std::ofstream& output) const override;
	std::ifstream& read(std::ifstream& input) override;

	void* __cdecl allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/) override;
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/) override;

	std::pair<int, int> getCurrentState() const;
	void showCurrentState() const;

	~BestFit();

private:
	bool checkBadAlloc(const size_t aSize, const std::list<PoolElement>::iterator blockAvailable);
	bool checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator blockToDeallocate);
	void checkMemoryLeaks();

	std::tuple<std::list<PoolElement>::iterator, std::list<PoolElement>::iterator> findAdjacentBlocks(const std::set<PoolElement>::iterator blockDeallocated);
	std::list<PoolElement>::iterator findBestBlock(const size_t aSize);
	void allocMemoryPool();
	void insertIntoAvailableMemory(const std::set<PoolElement>::iterator blockDeallocated);

private:
	char* startAddress;					// Start address of the memory pool
	std::list<PoolElement> mAvailable;	// Stores the memory unallocated sorted descending by the size
	std::set<PoolElement> mAllocated;	// Stores the memory allocated sorted by the address
	std::size_t poolSize;				// The size of the memory pool
};
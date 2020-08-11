#pragma once


/*
	Class used to implement a strategy for the allocator that uses the Buddy System algorithm to allocate and deallocate memory
*/
class BuddySystem final : public Strategy
{
public:
	BuddySystem(const size_t poolSize, const LogType logType);

	std::ofstream& write(std::ofstream& output) const override;
	std::ifstream& read(std::ifstream& input) override;

	void* __cdecl allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/) override;
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/) override;

	std::pair<int, int> getCurrentState() const;
	void showCurrentState() const;

	~BuddySystem();

private:
	bool checkBadAlloc(const size_t aSize, int position);
	bool checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator blockToDeallocate);
	void checkMemoryLeaks();

	void allocMemoryPool();
	void insertIntoAvailableMemory(PoolElement blockDeallocated);
	void mergeAdjacentBlocks(std::list<PoolElement>& blocks) const;
	PoolElement getAvailableBlock(const size_t aSize, int position);
	std::set<PoolElement>::iterator findAdjacentBlock(const PoolElement& blockDeallocated, const int position) const;

private:
	char* startAddress;
	std::vector<std::set<PoolElement>> mAvailable; 	// Stores the memory unallocated sorted ascending by the address
	std::set<PoolElement> mAllocated;			    // Stores the memory allocated sorted by the address
	std::size_t poolSize;							// The size of the memory pool
};
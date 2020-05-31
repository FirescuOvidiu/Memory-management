#pragma once


/*
	Class used to implement a custom allocator that uses the buddy system algorithm to allocate and deallocate memory
*/
class BuddySystem : public Strategy
{
public:
	BuddySystem(size_t poolSize);

	std::ostream& write(std::ostream& output) const override;
	std::istream& read(std::istream& input) override;

	void* __cdecl allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/) override;
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/) override;

	std::pair<int, int> getCurrentState() const;
	void showCurrentState() const;

	~BuddySystem();

private:
	bool checkBadAlloc(size_t aSize, int& position);
	bool checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& it);
	void checkMemoryLeaks();

	PoolElement getAvailableBlock(size_t aSize, int position);
	void insertIntoAvailableMemory(PoolElement& deallocatedMemory);
	void findAdjacentBlock(PoolElement& buddy, const PoolElement& deallocatedMemory) const;

private:
	char* startAddress;					// Start address of the memory pool
	std::vector<std::set<PoolElement>> mAvailable; 	// Stores the memory unallocated sorted ascending by the address
	std::set<PoolElement> mAllocated;	// Stores the memory allocated sorted by the address
	std::size_t poolSize;				// The size of the memory pool
};
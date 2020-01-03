#pragma once


class BuddySystem : public Strategy, public DiagnosticTools
{
public:
	BuddySystem(size_t poolSize);

	void* __cdecl allocMemory(size_t aSize, int aBlockUse, char const* /*aFileName*/, int /*aLineNumber*/) override;
	void __cdecl freeMemory(void* aBlock, int /*aBlockUse*/) override;

	~BuddySystem();

private:
	char* startAddress;					// Start address of the memory pool
	std::vector<std::set<PoolElement>> mAvailable;
	std::set<PoolElement> mAllocated;	// Stores the memory allocated sorted by the address
	std::size_t poolSize;				// The size of the memory pool
};
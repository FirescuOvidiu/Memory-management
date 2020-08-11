#include "stdafx.h"


BuddySystem::BuddySystem(const size_t poolSize, const LogType logType) : Strategy(poolSize, logType)
{
	// poolSize will be the first number that is a power of 2 and is smaller or equal with poolSize
	this->poolSize = (int)pow(2, (int)floor(log2((int)poolSize)));
	allocMemoryPool();

	mAvailable.resize((int)(log2(this->poolSize) + 1));
	mAvailable.back().insert(PoolElement(startAddress, this->poolSize));
}


/*
	Method used to serialize an object
*/
std::ofstream& BuddySystem::write(std::ofstream& output) const
{
	// Serialize data members by writing their content into the output file
	Writer::writeVariable(output, poolSize);
	Writer::writeVectorOfSets(output, mAvailable);
	Writer::writeSet(output, mAllocated);

	return output;
}


/*
	Method used to deserialize an object
*/
std::ifstream& BuddySystem::read(std::ifstream& input)
{
	// Deserialize data members by reading their content from the input file
	poolSize = Reader::readVariable<decltype(poolSize)>(input);

	allocMemoryPool();

	mAvailable = Reader::readVectorOfSets(input);
	mAllocated = Reader::readSet(input);

	return input;
}


/*
	The method returns an address to an open block of memory of size power of 2 that is equal or bigger than aSize from our memory pool
*/
void* __cdecl BuddySystem::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	// Position will be used to find an open block of memory 
	int position = (int)std::ceil(log2(aSize));

	// We parse memory available and search for a block of memory that is equal or bigger to aSize
	while ((position < (int)mAvailable.size()) && (mAvailable[position].empty()))
	{
		position++;
	}

	if (checkBadAlloc(aSize, position))
	{
		std::bad_alloc exception;
		throw exception;
	}

	auto blockAvailable = getAvailableBlock(aSize, position);

	mAllocated.insert(blockAvailable);

	// Update logger
	log.increaseAllocOrDealloc(-(int)blockAvailable.size);

	return blockAvailable.address;
}


/*
	Method used to inserts the block of memory deallocated into the memory available, and removes the block from memory allocated
*/
void __cdecl BuddySystem::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	auto blockToDeallocate = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, blockToDeallocate))
	{
		std::abort();
	}

	PoolElement blockDeallocated(blockToDeallocate->address, (int)pow(2, (int)std::ceil(log2(blockToDeallocate->size))));

	insertIntoAvailableMemory(blockDeallocated);

	// Update logger
	log.increaseAllocOrDealloc((int)blockDeallocated.size);

	mAllocated.erase(blockToDeallocate);
}


/*
	Method used to get the memory allocated and memory requested by the user at this point
	This variables are used to evaluate the fragmentation state of the custom allocator
*/
std::pair<int, int> BuddySystem::getCurrentState() const
{
	int memoryAllocated = std::reduce(std::cbegin(mAvailable), std::cend(mAvailable), (int)poolSize, [](int currSize, const auto& set)
		{
			return currSize - std::accumulate(std::cbegin(set), std::cend(set), 0, [](int currMemory, const auto& element)
				{
					return currMemory + (int)element.size;
				});
		});
	int memoryRequested = std::accumulate(std::cbegin(mAllocated), std::cend(mAllocated), 0, [](int currMemory, const auto& element)
		{
			return currMemory + (int)element.size;
		});

	return std::make_pair(memoryAllocated, memoryRequested);
}


/*
	Methods used to store into a file the state of the memory
	The file will be used to make a chart in excel showing the current state of memory
*/
void BuddySystem::showCurrentState() const
{
	std::ofstream output("memoryState.txt", std::ofstream::out);
	std::list<PoolElement> mAvailableCopy, mAllocatedCopy;

	for (const auto& blockAllocated : mAllocated)
	{
		mAllocatedCopy.push_back(PoolElement(blockAllocated.address, (int)pow(2, (int)std::ceil(log2(blockAllocated.size)))));
	}
	mergeAdjacentBlocks(mAllocatedCopy);

	// copy and sort the list of memory available by address then merge adjacent blocks
	for (const auto& blocksAvailable : mAvailable)
	{
		for (const auto& blockAvailable : blocksAvailable)
		{
			mAvailableCopy.push_back(blockAvailable);
		}
	}
	mAvailableCopy.sort();
	mergeAdjacentBlocks(mAvailableCopy);

	Writer::writeSortedLists(output, mAvailableCopy, mAllocatedCopy);
	
	output.close();
}


bool BuddySystem::checkBadAlloc(const size_t aSize, int position)
{
	if (position >= (int)mAvailable.size())
	{
		// Find the biggest continuous memory available
		position = (int)mAvailable.size() - 1;
		while ((position >= 0) && (mAvailable[position].empty()))
		{
			position--;
		}

		log.updateErrorLog(0, aSize, (int)pow(2, position), "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


bool BuddySystem::checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator blockToDeallocate)
{
	if (blockToDeallocate == std::end(mAllocated))
	{
		log.updateErrorLog(aBlock, 0, 0, "Invalid Address");
		log.~Logger();

		return true;
	}

	return false;
}


void BuddySystem::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		log.updateWarningLog();
	}
}


void BuddySystem::allocMemoryPool()
{
	if (startAddress != nullptr)
	{
		delete[] startAddress;
		startAddress = nullptr;
	}
	startAddress = new char[this->poolSize];
	PoolElement::setStartAddress(startAddress);
}


/*
	Insert the address that was removed from the allocated block into the available memory (mAvailable)
*/
void BuddySystem::insertIntoAvailableMemory(PoolElement blockDeallocated)
{
	int position = (int)std::ceil(log2(blockDeallocated.size));
	auto adjacentBlock = findAdjacentBlock(blockDeallocated, position);

	while (adjacentBlock != std::end(mAvailable[position]))
	{
		// Merge deallocated block of memory with the adjacent block
		if (adjacentBlock->address < blockDeallocated.address)
		{
			blockDeallocated.address = adjacentBlock->address;
		}
		blockDeallocated.size *= 2;

		mAvailable[position++].erase(adjacentBlock);

		// Find the new adjacent block of the two merged blocks
		adjacentBlock = findAdjacentBlock(blockDeallocated, position);
	}

	mAvailable[position].insert(blockDeallocated);
}


void BuddySystem::mergeAdjacentBlocks(std::list<PoolElement>& blocks) const
{
	std::list<PoolElement>::iterator it = std::begin(blocks);

	while ((it = std::adjacent_find(it, std::end(blocks), [](const auto& first, const auto& second)
		{
			return (first.address + first.size == second.address);
		})) != std::end(blocks))
	{
		std::next(it)->updateElement(it->address, it->size + std::next(it)->size);
		it = blocks.erase(it);
	}
}


/*
	Method used to find the open block that is of size:
	first number that is a power of 2 and is bigger or equal with the aSize
*/
PoolElement BuddySystem::getAvailableBlock(const size_t aSize, int position)
{
	PoolElement firstBlock, secondBlock, currBlock;

	// Get the open block from the memory available
	currBlock = *std::begin(mAvailable[position]);
	mAvailable[position].erase(std::begin(mAvailable[position]));

	// While half of current open block is bigger than aSize then 
	// we split the current open block into two blocks
	position--;
	while (pow(2, position) >= aSize)
	{
		// Split the current open block into two blocks
		firstBlock.updateElement(currBlock.address, currBlock.size / 2);
		secondBlock.updateElement(currBlock.address + currBlock.size / 2, currBlock.size / 2);

		// The first half becomes the current block
		currBlock = firstBlock;
		// The second half is inserted into the vector used to keep track of the available memory
		mAvailable[position].insert(secondBlock);
		position--;
	}

	currBlock.size = aSize;
	return currBlock;
}


/*
	Method used to find the adjacent block of the deallocated block of memory
*/
std::set<PoolElement>::iterator BuddySystem::findAdjacentBlock(const PoolElement& blockDeallocated, const int position) const
{
	PoolElement adjacentBlock;

	adjacentBlock.size = blockDeallocated.size;
	if (((blockDeallocated.address - startAddress) / blockDeallocated.size) % 2)
	{
		adjacentBlock.address = blockDeallocated.address - blockDeallocated.size;
	}
	else
	{
		adjacentBlock.address = blockDeallocated.address + blockDeallocated.size;
	}

	return mAvailable[position].find(adjacentBlock);
}


BuddySystem::~BuddySystem()
{
	checkMemoryLeaks();

	if (startAddress != nullptr)
	{
		delete[] startAddress;
		startAddress = nullptr;
	}
}
#include "stdafx.h"


BestFit::BestFit(const size_t poolSize) : Strategy(poolSize), poolSize(poolSize), startAddress(nullptr)
{
	allocMemoryPool();
	mAvailable.push_back(PoolElement(startAddress, poolSize));
}


/*
	Method used to serialize an object
*/
std::ofstream& BestFit::write(std::ofstream& output) const
{
	// Serialize data members by writing their content in the output file
	Writer::writeVariable(output, poolSize);
	Writer::writeList(output, mAvailable);
	Writer::writeSet(output, mAllocated);

	return output;
}


/*
	Method used to deserialize an object
*/
std::ifstream& BestFit::read(std::ifstream& input)
{
	// Deserialize data members by reading their content from the input file
	poolSize = Reader::readVariable<decltype(poolSize)>(input);

	allocMemoryPool();

	mAvailable = Reader::readList(input);
	mAllocated = Reader::readSet(input);

	return input;
}


/*
	Function used to allocate memory for the user
	Returns an address to an open block of memory of size aSize in our memory pool
*/
void* __cdecl BestFit::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	std::list<PoolElement>::iterator bestBlockAvailable = findBestBlock(aSize);
	
	if (checkBadAlloc(aSize,bestBlockAvailable))
	{
		std::bad_alloc exception;
		throw exception;
	}

	void* bestBlock = static_cast<void*>(bestBlockAvailable->address);

	// Insert the address and the size of the block in the memory allocated
	mAllocated.insert(PoolElement(bestBlockAvailable->address, aSize));

	// Update or erase the block based on the size of the block
	if (bestBlockAvailable->size != aSize)
	{
		bestBlockAvailable->updateElement(bestBlockAvailable->address + aSize, bestBlockAvailable->size - aSize);
	}
	else
	{
		mAvailable.erase(bestBlockAvailable);
	}

	// Update logger
	log.increaseAllocOrDealloc(-(int)aSize);

	return bestBlock;
}


/*
	Function used to deallocate memory. Does two things:
	- removes the address from the mAllocated (mAllocated contains the blocks allocated)
	- inserts the address into the mAvailable (mAvailable contains the blocks unallocated)
*/
void __cdecl BestFit::freeMemory(void* aBlock, int)
{
	// Search the address that the user wants to delete in mAllocated
	const auto itBlockToDeallocate = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, itBlockToDeallocate))
	{
		std::abort();
	}

	// Insert the address into the unallocated list (mAvailable)
	insertIntoAvailableMemory(itBlockToDeallocate);

	// Update Logger
	log.increaseAllocOrDealloc((int)itBlockToDeallocate->size);

	// Remove the adress from the allocated block
	mAllocated.erase(itBlockToDeallocate);
}


/*
	The method returns a pair with biggest contiguous memory and
	total memory available used to evaluate the fragmentation state of the custom allocator
*/
std::pair<int, int> BestFit::getCurrentState() const
{
	int biggestContMemory = findBiggestContBlock(), memoryAvailable = (int)poolSize;

	for (const auto& blockAllocated : mAllocated)
	{
		memoryAvailable -= (int)blockAllocated.size;
	}

	return std::make_pair(biggestContMemory, memoryAvailable);
}


/*
	Methods used to store into a file the state of the memory file
	The file will be imported into excel and make a chart with the information present
*/
void BestFit::showCurrentState() const
{
	std::ofstream output("memoryState.txt", std::ofstream::out);
	std::list<PoolElement> auxMAvailable = mAvailable, auxMAllocated;

	for (const auto& currBlockAllocated : mAllocated)
	{
		auxMAllocated.push_back(currBlockAllocated);
	}
	mergeAdjacentBlocks(auxMAllocated);

	// Sorting the list of memory available by address
	auxMAvailable.sort();

	Writer::writeSortedLists(output, auxMAvailable, auxMAllocated);

	output.close();
}


/*
	Destructor used to deallocated the memory pool allocated at the start of the program
	Also we check if the program has memory leaks
*/
BestFit::~BestFit()
{
	checkMemoryLeaks();

	if (startAddress != nullptr)
	{
		delete[] startAddress;
		startAddress = nullptr;
	}
}


/*
	Check if we can't allocate memory for the user because different reasons
*/
bool BestFit::checkBadAlloc(size_t aSize, std::list<PoolElement>::iterator& itBlockAvailable)
{
	// If we don't have enough memory available or 
	// The biggest contiguous memory is smaller than the memory requested
	if (itBlockAvailable == std::end(mAvailable))
	{

		log.updateErrorLog(0, aSize, findBiggestContBlock(), "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


/*
	Method used to check if the user tried to deallocate a block of memory which is not allocated
*/
bool BestFit::checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& itBlockToDeallocate)
{
	if (itBlockToDeallocate == std::end(mAllocated))
	{
		log.updateErrorLog(aBlock, 0, 0, "Invalid Address");
		log.~Logger();

		return true;
	}

	return false;
}


/*
	Methods used to check if the application has memory leaks
*/
void BestFit::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		log.updateWarningLog();
	}
}


void BestFit::allocMemoryPool()
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
	Insert the block of memory that was removed from the allocated set (mAllocated) into the unallocated list (mAvailable)
*/
void BestFit::insertIntoAvailableMemory(const std::set<PoolElement>::iterator& itDeallocatedMemory)
{
	// leftBlock will memorate the left block with which the deallocated block will merge
	// rightBlock will memorate the right block with which the deallocated block will merge

	auto [leftBlock, rightBlock] = findAdjacentBlocks(itDeallocatedMemory);

	// If it doesn't merge with other blocksinsert it at the end of list
	if ((leftBlock == std::end(mAvailable)) && (rightBlock == std::end(mAvailable)))
	{
		mAvailable.push_back(*itDeallocatedMemory);
		return;
	}

	// Merge the deallocated block with a left block and a right block
	// The right block is deleted and only the left block will remain with the size equal to sum of all 3 blocks
	if ((leftBlock != std::end(mAvailable)) && (rightBlock != std::end(mAvailable)))
	{
		leftBlock->size = leftBlock->size + rightBlock->size + itDeallocatedMemory->size;
		mAvailable.erase(rightBlock);
	}
	else
	{
		// Merge the deallocated block with the a left block
		if (leftBlock != std::end(mAvailable))
		{
			leftBlock->size = leftBlock->size + itDeallocatedMemory->size;
		}

		// Merge the deallocated block with a right block
		if (rightBlock != std::end(mAvailable))
		{
			rightBlock->address = itDeallocatedMemory->address;
			rightBlock->size = rightBlock->size + itDeallocatedMemory->size;
		}
	}
}


std::list<PoolElement>::iterator BestFit::findBestBlock(const size_t aSize)
{
	std::list<PoolElement>::iterator bestBlockAvailable = std::begin(mAvailable);

	while (bestBlockAvailable != std::end(mAvailable) && (int)bestBlockAvailable->size <= (int)aSize)
	{
		bestBlockAvailable++;
	}

	if (bestBlockAvailable != std::end(mAvailable))
	{
		for (auto nextAvailableBlock = std::next(bestBlockAvailable); nextAvailableBlock != std::end(mAvailable); nextAvailableBlock++)
		{
			if (((int)bestBlockAvailable->size >= (int)nextAvailableBlock->size) && ((int)nextAvailableBlock->size >= (int)aSize))
			{
				bestBlockAvailable = nextAvailableBlock;
			}
		}
	}

	return bestBlockAvailable;
}

std::tuple<std::list<PoolElement>::iterator, std::list<PoolElement>::iterator> BestFit::findAdjacentBlocks(const std::set<PoolElement>::iterator& itDeallocatedMemory)
{
	auto leftBlock = std::end(mAvailable), rightBlock = std::end(mAvailable);

	// We parse the list with the available blocks and check if the deallocated block 
	// can be merged with another block of memory from the available memory
	for (auto currElement = std::begin(mAvailable); currElement != std::end(mAvailable); currElement++)
	{
		if (currElement->address + currElement->size == itDeallocatedMemory->address)
		{
			leftBlock = currElement;
		}

		if (currElement->address == itDeallocatedMemory->address + itDeallocatedMemory->size)
		{
			rightBlock = currElement;
		}
	}

	return std::make_tuple(leftBlock, rightBlock);
}


void BestFit::mergeAdjacentBlocks(std::list<PoolElement>& blocks) const
{
	if (blocks.empty())
	{
		return;
	}

	auto currBlock = std::begin(blocks);

	while (std::next(currBlock) != std::end(blocks))
	{
		if (currBlock->address + currBlock->size == std::next(currBlock)->address)
		{
			std::next(currBlock)->updateElement(currBlock->address, currBlock->size + std::next(currBlock)->size);
			currBlock = blocks.erase(currBlock);
		}
		else
		{
			currBlock++;
		}
	}
}


int BestFit::findBiggestContBlock() const
{
	int biggestContBlock = 0;

	for (const auto& blockAvailable : mAvailable)
	{
		if (biggestContBlock < (int)blockAvailable.size)
		{
			biggestContBlock = (int)blockAvailable.size;
		}
	}

	return biggestContBlock;
}
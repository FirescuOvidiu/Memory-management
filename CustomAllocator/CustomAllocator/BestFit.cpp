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
	// Serialize data members by writing their content into the output file
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
	The method returns an address to an open block of memory of size equal or bigger than aSize from our memory pool
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
	Method used to inserts the block of memory deallocated into the memory available, and removes the block from memory allocated
*/
void __cdecl BestFit::freeMemory(void* aBlock, int)
{
	// Search the address that the user wants to delete in mAllocated
	const auto blockToDeallocate = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, blockToDeallocate))
	{
		std::abort();
	}

	insertIntoAvailableMemory(blockToDeallocate);

	// Update Logger
	log.increaseAllocOrDealloc((int)blockToDeallocate->size);

	// Remove the address from the allocated block
	mAllocated.erase(blockToDeallocate);
}


/*
	Returns a pair with biggest contiguous memory and
	total memory available used to evaluate the fragmentation state of the allocator
*/
std::pair<int, int> BestFit::getCurrentState() const
{
	const auto biggestContMemory = std::max_element(std::cbegin(mAvailable), std::cend(mAvailable), [](const auto& first, const auto& second)
		{
			return first.size < second.size;
		});
	const int memoryAvailable = std::reduce(std::cbegin(mAllocated), std::cend(mAllocated), (int)poolSize, [](int currSize, const auto& element)
			{
				return currSize - (int)element.size;
			});;

	return std::make_pair((int)biggestContMemory->size, memoryAvailable);
}


/*
	Methods used to store into a file the state of the memory
	The file will be used to make a chart in excel showing the current state of memory
*/
void BestFit::showCurrentState() const
{
	std::ofstream output("memoryState.txt", std::ofstream::out);
	std::list<PoolElement> copyMAvailable = mAvailable, copyMAllocated;

	for (const auto& currBlockAllocated : mAllocated)
	{
		copyMAllocated.push_back(currBlockAllocated);
	}

	// Merge adjacent blocks of memory
	std::list<PoolElement>::iterator it = copyMAllocated.begin();

	while ((it = std::adjacent_find(it, copyMAllocated.end(), [](const auto& first, const auto& second)
		{
			return (first.address + first.size == second.address);
		})) != copyMAllocated.end())
	{
		std::next(it)->updateElement(it->address, it->size + std::next(it)->size);
		it = copyMAllocated.erase(it);
	}

	copyMAvailable.sort();

	Writer::writeSortedLists(output, copyMAvailable, copyMAllocated);

	output.close();
}



BestFit::~BestFit()
{
	checkMemoryLeaks();

	if (startAddress != nullptr)
	{
		delete[] startAddress;
		startAddress = nullptr;
	}
}



bool BestFit::checkBadAlloc(const size_t aSize, const std::list<PoolElement>::iterator& blockAvailable)
{
	if (blockAvailable == std::end(mAvailable))
	{
		const auto biggestContMemory = std::max_element(std::cbegin(mAvailable), std::cend(mAvailable), [](const auto& first, const auto& second)
			{
				return first.size < second.size;
			});

		log.updateErrorLog(0, aSize, (int)biggestContMemory->size, "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


bool BestFit::checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& blockToDeallocate)
{
	if (blockToDeallocate == std::end(mAllocated))
	{
		log.updateErrorLog(aBlock, 0, 0, "Invalid Address");
		log.~Logger();

		return true;
	}

	return false;
}


void BestFit::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		log.updateWarningLog();
	}
}


std::tuple<std::list<PoolElement>::iterator, std::list<PoolElement>::iterator> BestFit::findAdjacentBlocks(const std::set<PoolElement>::iterator& blockDeallocated)
{
	auto leftBlock = std::end(mAvailable), rightBlock = std::end(mAvailable);

	// We parse the available blocks and save the blocks that can be merged with the deallocated block
	for (auto currElement = std::begin(mAvailable); currElement != std::end(mAvailable); currElement++)
	{
		if (currElement->address + currElement->size == blockDeallocated->address)
		{
			leftBlock = currElement;
		}

		if (currElement->address == blockDeallocated->address + blockDeallocated->size)
		{
			rightBlock = currElement;
		}
	}

	return std::make_tuple(leftBlock, rightBlock);
}


std::list<PoolElement>::iterator BestFit::findBestBlock(const size_t aSize)
{
	auto bestBlockAvailable = std::find_if(std::begin(mAvailable), std::end(mAvailable), [aSize](const auto& block)
		{
			return (int)block.size > (int)aSize;
		});

	bestBlockAvailable = std::min_element(bestBlockAvailable, std::end(mAvailable), [aSize](const auto& first, const auto& second)
		{
			return (((int)first.size < (int)second.size) && ((int)first.size >= (int)aSize));
		});

	return bestBlockAvailable;
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
void BestFit::insertIntoAvailableMemory(const std::set<PoolElement>::iterator& blockDeallocated)
{
	auto [leftBlock, rightBlock] = findAdjacentBlocks(blockDeallocated);

	// If it doesn't merge with other blocks, insert it at the end of list
	if ((leftBlock == std::end(mAvailable)) && (rightBlock == std::end(mAvailable)))
	{
		mAvailable.push_back(*blockDeallocated);
		return;
	}

	// Merge the deallocated block with a left block and a right block
	if ((leftBlock != std::end(mAvailable)) && (rightBlock != std::end(mAvailable)))
	{
		leftBlock->size = leftBlock->size + rightBlock->size + blockDeallocated->size;
		mAvailable.erase(rightBlock);
	}
	else
	{
		// Merge the deallocated block with the a left block
		if (leftBlock != std::end(mAvailable))
		{
			leftBlock->size = leftBlock->size + blockDeallocated->size;
		}

		// Merge the deallocated block with a right block
		if (rightBlock != std::end(mAvailable))
		{
			rightBlock->updateElement(blockDeallocated->address, rightBlock->size + blockDeallocated->size);
		}
	}
}
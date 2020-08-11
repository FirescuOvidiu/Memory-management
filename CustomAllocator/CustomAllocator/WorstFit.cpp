#include "stdafx.h"


WorstFit::WorstFit(const size_t poolSize, const LogType logType) : Strategy(poolSize, logType), poolSize(poolSize), startAddress(nullptr)
{
	allocMemoryPool();
	mAvailable.push_back(PoolElement(startAddress, poolSize));
}


/*
	Method used to serialize an object
*/
std::ofstream& WorstFit::write(std::ofstream& output) const
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
std::ifstream& WorstFit::read(std::ifstream& input)
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
void* __cdecl WorstFit::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	if (checkBadAlloc(aSize))
	{
		std::bad_alloc exception;
		throw exception;
	}

	auto blockAvailable = mAvailable.front();

	mAllocated.insert(PoolElement(blockAvailable.address, aSize));

	// Update the block with the new address and size and mantain the list sorted 
	mAvailable.front().updateElement(blockAvailable.address + aSize, blockAvailable.size - aSize);
	maintainListSorted(std::begin(mAvailable));

	// Update logger
	log.increaseAllocOrDealloc(-(int)aSize);

	return static_cast<void*>(blockAvailable.address);
}


/*
	Method used to inserts the block of memory deallocated into the memory available, and removes the block from memory allocated
*/
void __cdecl WorstFit::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	const auto blockToDeallocate = mAllocated.find(PoolElement(static_cast<char*>(aBlock)));

	if (checkInvalidAddress(aBlock, blockToDeallocate))
	{
		std::abort();
	}

	insertIntoAvailableMemory(blockToDeallocate);

	// Update Logger
	log.increaseAllocOrDealloc((int)blockToDeallocate->size);

	mAllocated.erase(blockToDeallocate);
}


/*
	Returns a pair with biggest contiguous memory and
	total memory available used to evaluate the fragmentation state of the allocator
*/
std::pair<int, int> WorstFit::getCurrentState() const
{
	const int memoryAvailable = std::reduce(std::cbegin(mAllocated), std::cend(mAllocated), (int)poolSize, [](int currSize, const auto& element)
		{
			return currSize - (int)element.size;
		});
	int biggestContMemory = mAvailable.empty() ? 0 : (int)mAvailable.front().size;

	return std::make_pair(biggestContMemory, memoryAvailable);
}


/*
	Methods used to store into a file the state of the memory
	The file will be used to make a chart in excel showing the current state of memory
*/
void WorstFit::showCurrentState() const
{
	std::ofstream output("memoryState.txt", std::ofstream::out);
	std::list<PoolElement> mAvailableCopy = mAvailable, mAllocatedCopy;

	for (const auto& blockAllocated : mAllocated)
	{
		mAllocatedCopy.push_back(blockAllocated);
	}

	// Merge adjacent blocks of memory
	std::list<PoolElement>::iterator it = mAllocatedCopy.begin();

	while ((it = std::adjacent_find(it, std::end(mAllocatedCopy), [](const auto& first, const auto& second)
		{
			return (first.address + first.size == second.address);
		})) != std::end(mAllocatedCopy))
	{
		std::next(it)->updateElement(it->address, it->size + std::next(it)->size);
		it = mAllocatedCopy.erase(it);
	}

	mAvailableCopy.sort();

	Writer::writeSortedLists(output, mAvailableCopy, mAllocatedCopy);

	output.close();
}


bool WorstFit::checkBadAlloc(size_t aSize)
{
	if (mAvailable.empty() || (aSize > mAvailable.front().size))
	{
		log.updateErrorLog(0, aSize, mAvailable.front().size, "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


bool WorstFit::checkInvalidAddress(void* aBlock,const std::set<PoolElement>::iterator blockToDeallocate)
{
	if (blockToDeallocate == std::end(mAllocated))
	{
		log.updateErrorLog(aBlock, 0, 0, "Invalid Address");
		log.~Logger();

		return true;
	}

	return false;
}


void WorstFit::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		log.updateWarningLog();
	}
}


void WorstFit::allocMemoryPool()
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
	Function used to maintain the list sorted descending by size
	We have the list sorted except one element that has the size bigger than the previous elements
	we try to find its position so that the list is maintained sorted descending by size
*/
void WorstFit::maintainListSorted(std::list<PoolElement>::iterator element)
{
	if (element->size == 0)
	{
		mAvailable.erase(element);
		return;
	}

	while ((element != std::begin(mAvailable)) && (element->size > std::prev(element)->size))
	{
		std::swap(element->address, std::prev(element)->address);
		std::swap(element->size, std::prev(element)->size);
		element--;
	}

	while ((std::next(element) != std::end(mAvailable)) && (element->size < std::next(element)->size))
	{
		std::swap(element->address, std::next(element)->address);
		std::swap(element->size, std::next(element)->size);
		element++;
	}
}


/*
	Insert the block of memory that was removed from the allocated set (mAllocated) into the unallocated list (mAvailable)
*/
void WorstFit::insertIntoAvailableMemory(const std::set<PoolElement>::iterator blockDeallocated)
{
	auto [newPosition, leftBlock, rightBlock] = findPosAndAdjacentBlocks(blockDeallocated);

	// If it doesn't merge with other blocks we insert it so that the lists is mantained sorted
	if ((leftBlock == std::end(mAvailable)) && (rightBlock == std::end(mAvailable)))
	{
		mAvailable.insert(newPosition, *blockDeallocated);
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
			leftBlock = rightBlock;
		}
	}
	maintainListSorted(leftBlock);
}


/*
	Method used to get the position where the deallocated block will fit
	Also inside the method we are looking for a left and right block to merge it with the block deallocated
*/
std::tuple<std::list<PoolElement>::iterator, std::list<PoolElement>::iterator, std::list<PoolElement>::iterator> 
	WorstFit::findPosAndAdjacentBlocks(const std::set<PoolElement>::iterator blockDeallocated)
{
	auto newPosition = std::begin(mAvailable), leftBlock = std::end(mAvailable), rightBlock = std::end(mAvailable);

	for (auto currElement = std::begin(mAvailable); currElement != std::end(mAvailable); currElement++)
	{
		if (newPosition->size > blockDeallocated->size)
		{
			newPosition++;
		}

		if (currElement->address + currElement->size == blockDeallocated->address)
		{
			leftBlock = currElement;
		}

		if (currElement->address == blockDeallocated->address + blockDeallocated->size)
		{
			rightBlock = currElement;
		}
	}

	return std::make_tuple(newPosition, leftBlock, rightBlock);
}


WorstFit::~WorstFit()
{
	checkMemoryLeaks();

	if (startAddress != nullptr)
	{
		delete[] startAddress;
		startAddress = nullptr;
	}
}
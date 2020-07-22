#include "stdafx.h"


WorstFit::WorstFit(const size_t poolSize) : Strategy(poolSize), poolSize(poolSize), startAddress(nullptr)
{
	allocMemoryPool();
	mAvailable.push_back(PoolElement(startAddress, poolSize));
}


/*
	Method used to serialize an object
*/
std::ofstream& WorstFit::write(std::ofstream& output) const
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
	Method used to allocate memory
	Returns an address to an open block of memory of size aSize from our memory pool
*/
void* __cdecl WorstFit::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	if (checkBadAlloc(aSize))
	{
		std::bad_alloc exception;
		throw exception;
	}

	auto blockAvailable = mAvailable.front();

	// Insert the address and the size of the block in the memory allocated
	mAllocated.insert(PoolElement(blockAvailable.address, aSize));

	// Update the block with the new address and size and mantain the list sorted 
	mAvailable.front().updateElement(blockAvailable.address + aSize, blockAvailable.size - aSize);
	maintainListSorted(std::begin(mAvailable));

	// Update logger
	log.increaseAllocOrDealloc(-(int)aSize);

	return static_cast<void*>(blockAvailable.address);
}


/*
	Method used to deallocate memory
	Deallocates a block of memory with the address aBlock from our memory pool allocated previously
*/
void __cdecl WorstFit::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	const auto itBlockToDeallocate = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, itBlockToDeallocate))
	{
		std::abort();
	}

	// Insert the block in the unallocated list (mAvailable)
	insertIntoAvailableMemory(itBlockToDeallocate);

	// Update Logger
	log.increaseAllocOrDealloc((int)itBlockToDeallocate->size);

	// Remove the block from the allocated set
	mAllocated.erase(itBlockToDeallocate);
}


/*
	The method returns a pair with biggest contiguous memory and
	total memory available used to evaluate the fragmentation state of the custom allocator
*/
std::pair<int, int> WorstFit::getCurrentState() const
{
	int biggestContMemory = 0, memoryAvailable = (int)poolSize;

	for (const auto& blockAllocated : mAllocated)
	{
		memoryAvailable -= (int)blockAllocated.size;
	}

	if (!mAvailable.empty())
	{
		biggestContMemory = (int)mAvailable.front().size;
	}

	return std::make_pair(biggestContMemory, memoryAvailable);
}


/*
	Methods used to store into a file the state of the memory file
	The file will be imported into excel and make a chart with the information present
*/
void WorstFit::showCurrentState() const
{
	std::ofstream output("memoryState.txt", std::ofstream::out);
	std::list<PoolElement> auxMAvailable = mAvailable, auxMAllocated;

	for (const auto& blockAllocated : mAllocated)
	{
		auxMAllocated.push_back(blockAllocated);
	}
	mergeAdjacentBlocks(auxMAllocated);

	// Sorting the list of memory available by address
	auxMAvailable.sort();

	Writer::writeSortedLists(output, auxMAvailable, auxMAllocated);

	output.close();
}


/*
	Method used to check if we can't allocate memory for the user
*/
bool WorstFit::checkBadAlloc(size_t aSize)
{
	// If we don't have enough memory available or the biggest contiguous memory is smaller than the memory requested then we can't allocate memory
	if (mAvailable.empty() || (aSize > mAvailable.front().size))
	{
		log.updateErrorLog(0, aSize, mAvailable.front().size, "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


/*
	Method used to check if the user tried to deallocate a block of memory which is not allocated
*/
bool WorstFit::checkInvalidAddress(void* aBlock,const std::set<PoolElement>::iterator& itBlockToDeallocate)
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
	Function used to check if the program has memory leaks
*/
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
void WorstFit::insertIntoAvailableMemory(const std::set<PoolElement>::iterator& deallocatedMemory)
{
	// newPosition will memorate the position where the block will fit if it doesn't merge with other blocks
	// leftBlock will memorate the left block with which the deallocated block will merge
	// rightBlock will memorate the right block with which the deallocated block will merge

	auto [newPosition, leftBlock, rightBlock] = findPosAndAdjacentBlocks(deallocatedMemory);

	// If it doesn't merge with other blocks (to make a bigger continuous memory block)
	// we simply insert it so that the lists is mantained sorted
	if ((leftBlock == std::end(mAvailable)) && (rightBlock == std::end(mAvailable)))
	{
		mAvailable.insert(newPosition, *deallocatedMemory);
		return;
	}

	// Merge the deallocated block with a left block and a right block
	// The right block is deleted and only the left block will remain with the size equal to sum of all 3 blocks
	if ((leftBlock != std::end(mAvailable)) && (rightBlock != std::end(mAvailable)))
	{
		leftBlock->size = leftBlock->size + rightBlock->size + deallocatedMemory->size;
		mAvailable.erase(rightBlock);

		maintainListSorted(leftBlock);
	}
	else
	{
		// Merge the deallocated block with the a left block
		if (leftBlock != std::end(mAvailable))
		{
			leftBlock->size = leftBlock->size + deallocatedMemory->size;

			maintainListSorted(leftBlock);
		}

		// Merge the deallocated block with a right block
		if (rightBlock != std::end(mAvailable))
		{
			rightBlock->address = deallocatedMemory->address;
			rightBlock->size = rightBlock->size + deallocatedMemory->size;

			maintainListSorted(rightBlock);
		}
	}
}


/*
	Method used to merge all the adjacent blocks in a sorted list by address
*/
void WorstFit::mergeAdjacentBlocks(std::list<PoolElement>& blocks) const
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


/*
	Method used to get the position where the deallocated block will fit
	Also inside the method we are looking for a left and right block to merge it with the deallocate block
*/
std::tuple<std::list<PoolElement>::iterator, std::list<PoolElement>::iterator, std::list<PoolElement>::iterator> 
	WorstFit::findPosAndAdjacentBlocks(const std::set<PoolElement>::iterator& deallocatedMemory)
{
	auto newPosition = std::begin(mAvailable), leftBlock = std::end(mAvailable), rightBlock = std::end(mAvailable);

	for (auto currElement = std::begin(mAvailable); currElement != std::end(mAvailable); currElement++)
	{
		if (newPosition->size > deallocatedMemory->size)
		{
			newPosition++;
		}

		if (currElement->address + currElement->size == deallocatedMemory->address)
		{
			leftBlock = currElement;
		}

		if (currElement->address == deallocatedMemory->address + deallocatedMemory->size)
		{
			rightBlock = currElement;
		}
	}

	return std::make_tuple(newPosition,leftBlock,rightBlock);
}


/*
	Destructor used to deallocated the memory pool allocated at the start of the program
	Also we check if the program has memory leaks
*/
WorstFit::~WorstFit()
{
	checkMemoryLeaks();

	if (startAddress != nullptr)
	{
		delete[] startAddress;
		startAddress = nullptr;
	}
}
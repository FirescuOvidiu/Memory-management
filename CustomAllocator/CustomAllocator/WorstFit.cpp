#include "stdafx.h"


WorstFit::WorstFit(size_t poolSize) : poolSize(poolSize), startAddress(nullptr)
{
	setStartAddress();
	mAvailable.push_back(PoolElement(startAddress, poolSize));

	log.initLogger(poolSize);
}


/*
	Method used to serialize an object
*/
std::ofstream& WorstFit::write(std::ofstream& output) const
{
	// Serialize the data members by writing their content in the output file
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

	setStartAddress();

	mAvailable = Reader::readList(input);
	mAllocated = Reader::readSet(input);

	return input;
}


/*
	Function used to allocate memory for the user
	Returns an address to an open block of memory of size aSize in our memory pool
*/
void* __cdecl WorstFit::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	// Throw exception in case we can't allocate the memory because different reasons (see function)
	if (checkBadAlloc(aSize))
	{
		std::bad_alloc exception;
		throw exception;
	}

	auto currBlockAvailable = mAvailable.begin();
	void* block = static_cast<void*>(currBlockAvailable->address);

	// Insert the address and the size of the block in the memory allocated
	mAllocated.insert(PoolElement(currBlockAvailable->address, aSize));

	// Update the block with the new address and size and mantain the list sorted 
	mAvailable.front().updateElement(currBlockAvailable->address + aSize, currBlockAvailable->size - aSize);
	if (mAvailable.front().size == 0)
	{
		mAvailable.erase(mAvailable.begin());
	}
	else
	{
		while ((std::next(currBlockAvailable) != mAvailable.end()) && (currBlockAvailable->size < std::next(currBlockAvailable)->size))
		{
			std::swap(currBlockAvailable->address, std::next(currBlockAvailable)->address);
			std::swap(currBlockAvailable->size, std::next(currBlockAvailable)->size);
			currBlockAvailable++;
		}
	}

	// Update logger
	log.increaseAllocOrDealloc(-(int)aSize);

	return block;
}


/*
	Function used to deallocate memory. Does two things:
	- removes the address from the mAllocated (mAllocated contains the blocks allocated)
	- inserts the address into the mAvailable (mAvailable contains the blocks unallocated)
*/
void __cdecl WorstFit::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	// Search the address that the user wants to delete in mAllocated
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, it))
	{
		std::abort();
	}

	PoolElement deallocatedMemory = *it;

	// Remove the adress from the allocated block
	mAllocated.erase(it);

	// Insert the address into the unallocated list (mAvailable)
	insertIntoAvailableMemory(deallocatedMemory);

	// Update Logger
	log.increaseAllocOrDealloc((int)deallocatedMemory.size);
}


/*
	The method returns a pair with biggest contiguous memory and
	total memory available used to evaluate the fragmentation state of the custom allocator
*/
std::pair<int, int> WorstFit::getCurrentState() const
{
	int biggestContMemory = 0, memoryAvailable = 0, memoryAllocated = 0;

	for (const auto& itMemoryAllocated : mAllocated)
	{
		memoryAllocated += (int)itMemoryAllocated.size;
	}

	memoryAvailable = (int)poolSize - memoryAllocated;
	if (!mAvailable.empty())
	{
		biggestContMemory = (int)(*(mAvailable.begin())).size;
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

	for (const auto& currBlockAllocated : mAllocated)
	{
		auxMAllocated.push_back(currBlockAllocated);
	}

	for (auto currAllocatedBlock = auxMAllocated.begin(); std::next(currAllocatedBlock) != auxMAllocated.end();)
	{
		if (currAllocatedBlock->address + currAllocatedBlock->size == std::next(currAllocatedBlock)->address)
		{
			std::next(currAllocatedBlock)->updateElement(currAllocatedBlock->address, currAllocatedBlock->size + std::next(currAllocatedBlock)->size);
			currAllocatedBlock = auxMAllocated.erase(currAllocatedBlock);
		}
		else
		{
			currAllocatedBlock++;
		}
	}

	// Sorting the list of memory available by address
	auxMAvailable.sort();

	std::list<PoolElement>::const_iterator currAvailableBlock = auxMAvailable.cbegin();
	std::list<PoolElement>::const_iterator currAllocatedBlock = auxMAllocated.cbegin();

	if (currAvailableBlock != auxMAvailable.cend() && currAllocatedBlock != auxMAllocated.cend())
	{
		if (currAvailableBlock->address - startAddress > currAllocatedBlock->address - startAddress)
		{
			output << 0 << "\n";
		}
	}

	// We have two structures sorted by address and we need to write 
	// into a file the elements from the both structures sorted by address
	// We parse the structures simultaneous and compare the elements
	while (currAvailableBlock != auxMAvailable.cend() && currAllocatedBlock != auxMAllocated.cend())
	{
		if (currAvailableBlock->address - startAddress < currAllocatedBlock->address - startAddress)
		{
			output << currAvailableBlock->size << "\n";
			output << currAllocatedBlock->size << "\n";
		}
		else
		{
			output << currAllocatedBlock->size << "\n";
			output << currAvailableBlock->size << "\n";
		}

		currAvailableBlock++;
		currAllocatedBlock++;
	}

	if (currAvailableBlock != auxMAvailable.cend())
	{
		output << currAvailableBlock->size << "\n";
	}
	if (currAllocatedBlock != auxMAllocated.cend())
	{
		output << currAllocatedBlock->size << "\n";
	}

	output.close();
}


/*
	Check if we can't allocate memory for the user because different reasons
*/
bool WorstFit::checkBadAlloc(size_t aSize)
{
	// If we don't have enough memory available or 
	// The biggest contiguous memory is smaller than the memory requested
	if (mAvailable.empty()|| (aSize > mAvailable.front().size))
	{
		// Update log
		log.updateErrorLog(0, aSize, mAvailable.front().size, "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


/*
	This function is used to check if the user tried to deallocate a block of memory which is not allocated
*/
bool WorstFit::checkInvalidAddress(void* aBlock,const std::set<PoolElement>::iterator& it)
{
	if (it == mAllocated.end())
	{
		// Update log
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
		// Update log
		log.updateWarningLog();
	}
}


void WorstFit::setStartAddress()
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
void WorstFit::maintainListSorted(std::list<PoolElement>::iterator& element)
{
	while ((element != mAvailable.begin()) && (element->size > std::prev(element)->size))
	{
		std::swap(element->address, std::prev(element)->address);
		std::swap(element->size, std::prev(element)->size);
		element--;
	}
}


/*
	Insert the block of memory that was removed from the allocated set (mAllocated) into the unallocated list (mAvailable)
*/
void WorstFit::insertIntoAvailableMemory(const PoolElement& deallocatedMemory)
{
	// newPosition will memorate the position where the block will fit if it doesn't merge with other blocks
	// leftBlock will memorate the left block with which the deallocated block will merge
	// rightBlock will memorate the right block with which the deallocated block will merge
	std::list<PoolElement>::iterator newPosition = mAvailable.begin();
	std::list<PoolElement>::iterator leftBlock = mAvailable.end(), rightBlock = mAvailable.end();

	// We parse the list and find newPosition, leftBlock and rightBlock
	for (std::list<PoolElement>::iterator currElement = mAvailable.begin(); currElement != mAvailable.end(); currElement++)
	{
		if (newPosition->size > deallocatedMemory.size)
		{
			newPosition++;
		}

		if (currElement->address + currElement->size == deallocatedMemory.address)
		{
			leftBlock = currElement;
		}

		if (deallocatedMemory.address + deallocatedMemory.size == currElement->address)
		{
			rightBlock = currElement;
		}
	}

	// If it doesn't merge with other blocks (to make a bigger continuous memory block)
	// we simply insert it so that the lists is mantained sorted
	if ((leftBlock == mAvailable.end()) && (rightBlock == mAvailable.end()))
	{
		mAvailable.insert(newPosition, deallocatedMemory);
	}
	else
	{
		// Merge the deallocated block with a left block and a right block
		// The right block is deleted and only the left block will remain with the size equal to sum of all 3 blocks
		if ((leftBlock != mAvailable.end()) && (rightBlock != mAvailable.end()))
		{
			leftBlock->size = leftBlock->size + rightBlock->size + deallocatedMemory.size;
			mAvailable.erase(rightBlock);

			WorstFit::maintainListSorted(leftBlock);
		}
		else
		{
			// Merge the deallocated block with the a left block
			if (leftBlock != mAvailable.end())
			{
				leftBlock->size = leftBlock->size + deallocatedMemory.size;

				WorstFit::maintainListSorted(leftBlock);
			}

			// Merge the deallocated block with a right block
			if (rightBlock != mAvailable.end())
			{
				rightBlock->address = deallocatedMemory.address;
				rightBlock->size = rightBlock->size + deallocatedMemory.size;

				WorstFit::maintainListSorted(rightBlock);
			}
		}
	}
}


/*
	Destructor used to deallocated the memory pool, allocated at the start of the program
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
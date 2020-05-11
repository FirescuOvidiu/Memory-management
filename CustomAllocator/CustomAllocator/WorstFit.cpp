#include "stdafx.h"

/*
	Constructor used to allocate the memory pool, initialize data members and
	save the first address for deallocation after the T.U. is finished in case of memory leaks
 */
WorstFit::WorstFit(size_t poolSize) : poolSize(poolSize)
{
	startAddress = new char[poolSize];
	mAvailable.push_back(PoolElement(startAddress, poolSize));

	// Initialize data members of logger
	log.initLogger(poolSize, startAddress);
}


/*
	Method used to serialize an object
*/
std::ostream& WorstFit::write(std::ostream& output) const
{
	size_t lengthMAvailable = mAvailable.size(), lengthMAllocated = mAllocated.size();

	// Serialize the data members by writing their content in the output file
	output.write(reinterpret_cast<const char*>(&poolSize), sizeof(poolSize));

	// Write the length of the list
	output.write(reinterpret_cast<const char*>(&lengthMAvailable), sizeof(lengthMAvailable));
	// Parse the list and write it's content into the output file
	for (const auto& currAvailableBlock : mAvailable)
	{
		output << currAvailableBlock;
	}

	// Write the length of the set
	output.write(reinterpret_cast<const char*>(&lengthMAllocated), sizeof(lengthMAllocated));
	// Parse the set and write it's content into the output file
	for (const auto& currAllocatedBlock : mAllocated)
	{
		output << currAllocatedBlock;
	}

	return output;
}


/*
	Method used to deserialize an object
*/
std::istream& WorstFit::read(std::istream& input)
{
	size_t lengthMAvailable = 0, lengthMAllocated = 0;
	std::set<PoolElement> aux;
	PoolElement poolElement;

	// Deserialize data members by reading their content from the input file
	input.read(reinterpret_cast<char*>(&poolSize), sizeof(poolSize));

	// Delete the current allocated objects and allocate and initialize them 
	if (startAddress != nullptr)
	{
		delete startAddress;
	}
	startAddress = new char[this->poolSize];
	PoolElement::setStartAddress(startAddress);

	// Delete the list, and read it's length from the input file
	mAvailable.clear();
	input.read(reinterpret_cast<char*>(&lengthMAvailable), sizeof(lengthMAvailable));
	mAvailable.resize(lengthMAvailable);
	// Parse the vector and read it's content from the input file
	for (auto& currAvailableBlock : mAvailable)
	{
		input >> currAvailableBlock;
	}

	// Delete the set and read it's length
	mAllocated.clear();
	input.read(reinterpret_cast<char*>(&lengthMAllocated), sizeof(lengthMAllocated));
	// Insert into the set the elements read from the file
	for (int it = 0; it < (int)lengthMAllocated; it++)
	{
		input >> poolElement;
		mAllocated.insert(poolElement);
	}

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

	auto currBlock = mAvailable.begin();
	void* block = static_cast<void*>(currBlock->address);

	// Insert the address and the size of the block in the memory allocated
	mAllocated.insert(PoolElement(currBlock->address, aSize));

	// Update the block with the new address and size and mantain the list sorted 
	mAvailable.front().updateElement(currBlock->address + aSize, currBlock->size - aSize);
	while ((std::next(currBlock) != mAvailable.end()) && (currBlock->size < std::next(currBlock)->size))
	{
		std::swap(currBlock->address, std::next(currBlock)->address);
		std::swap(currBlock->size, std::next(currBlock)->size);
		currBlock++;
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
	Method used to get the memory allocated and memory requested by the user at this point
	This variables are used to evaluate the fragmentation state of the custom allocator
*/
std::pair<int, int> WorstFit::getCurrentState() const
{
	int biggestContMemory = 0, memoryAvailable = 0, memoryAllocated = 0;

	for (const auto& itMemoryAllocated : mAllocated)
	{
		memoryAllocated += (int)itMemoryAllocated.size;
	}

	memoryAvailable = (int)poolSize - memoryAllocated;
	biggestContMemory = (int)(*(mAvailable.begin())).size;

	return std::make_pair(biggestContMemory, memoryAvailable);
}


void WorstFit::showCurrentState() const
{
	std::ofstream output("worstFitState.txt", std::ofstream::out);
	std::list<PoolElement> auxMAvailable = mAvailable;
	PoolElement previousBlock(startAddress, 0);

	auxMAvailable.sort();

	for (const auto& currAvailableBlock : auxMAvailable)
	{
		if (currAvailableBlock.address != startAddress)
		{
			output << currAvailableBlock.address - previousBlock.address + previousBlock.size << "\n";
		}
		output << currAvailableBlock.size << "\n";
		previousBlock = currAvailableBlock;
	}

	if ((!auxMAvailable.empty()) && (auxMAvailable.back().address + auxMAvailable.back().size != startAddress + poolSize))
	{
		output << startAddress + poolSize - auxMAvailable.back().address + auxMAvailable.back().size << "\n";
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
		log.updateWarningLog(poolSize);
	}
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
	Insert the address that was removed from the allocated block into the unallocated list (mAvailable)
*/
void WorstFit::insertIntoAvailableMemory(const PoolElement& deletedMemory)
{

	std::list<PoolElement>::iterator newPosition = mAvailable.begin();
	std::list<PoolElement>::iterator leftBlock = mAvailable.end(), rightBlock = mAvailable.end();

	// We parse the list with the available blocks and do two things
	// - check if the deleted block can be merged with another block of memory from the available memory (we have 3 cases here)
	// - memorate the position where the block will fit if it doesn't need to merge with other blocks
	for (std::list<PoolElement>::iterator currElement = mAvailable.begin(); currElement != mAvailable.end(); currElement++)
	{
		if (newPosition->size > deletedMemory.size)
		{
			newPosition++;
		}

		if (currElement->address + currElement->size == deletedMemory.address)
		{
			leftBlock = currElement;
		}

		if (deletedMemory.address + deletedMemory.size == currElement->address)
		{
			rightBlock = currElement;
		}
	}

	// If it doesn't merge with other blocks (to make a bigger continuous memory block)
	// we simply insert it so that the lists is mantained sorted
	if ((leftBlock == mAvailable.end()) && (rightBlock == mAvailable.end()))
	{
		mAvailable.insert(newPosition, deletedMemory);
	}
	else
	{
		// Merge the deleted block with a left block and a right block
		// The right block is deleted and only the left block will remain with the size of all 3 blocks
		if ((leftBlock != mAvailable.end()) && (rightBlock != mAvailable.end()))
		{
			leftBlock->size = leftBlock->size + rightBlock->size + deletedMemory.size;
			mAvailable.erase(rightBlock);

			WorstFit::maintainListSorted(leftBlock);
		}
		else
		{
			// Merge the deleted block with the a left block
			if (leftBlock != mAvailable.end())
			{
				leftBlock->size = leftBlock->size + deletedMemory.size;

				WorstFit::maintainListSorted(leftBlock);
			}

			// Merge the deleted block with a right block
			if (rightBlock != mAvailable.end())
			{
				rightBlock->address = deletedMemory.address;
				rightBlock->size = rightBlock->size + deletedMemory.size;

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

	delete[] startAddress;
}
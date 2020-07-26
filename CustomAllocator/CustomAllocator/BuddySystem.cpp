#include "stdafx.h"


/*
	Constructor used to allocate the memory pool, initialize data members and
	save the start address to deallocate the memory pool and check for memory leaks after the T.U. is finished
 */
BuddySystem::BuddySystem(size_t poolSize, const LogType logType) : Strategy(poolSize, logType)
{
	// this->poolSize will be the first number that is a power of 2 and is smaller or equal with poolSize
	// If poolSize is a power of 2 then this->poolSize will be equal with poolSize
	this->poolSize = (int)pow(2, (int)floor(log2((int)poolSize)));
	startAddress = new char[this->poolSize];
	PoolElement::setStartAddress(startAddress);

	mAvailable.resize((int)(log2(this->poolSize) + 1));
	mAvailable[mAvailable.size() - 1].insert(PoolElement(startAddress, this->poolSize));
}


/*
	Method used to serialize an object
*/
std::ofstream& BuddySystem::write(std::ofstream& output) const
{
	size_t lengthMAvailable = mAvailable.size(), lengthMAllocated = mAllocated.size();

	// Serialize the data members by writing their content in the output file
	output.write(reinterpret_cast<const char*>(&poolSize), sizeof(poolSize));

	// Write the length of the vector
	output.write(reinterpret_cast<const char*>(&lengthMAvailable), sizeof(lengthMAvailable));
	// Parse the vector
	for (const auto& itMemoryAvailable : mAvailable)
	{
		// Write the length of the set
		lengthMAvailable = itMemoryAvailable.size();
		output.write(reinterpret_cast<const char*>(&lengthMAvailable), sizeof(lengthMAvailable));

		// Parse the set and write it's content into the output file
		for (const auto& currAvailableBlock : itMemoryAvailable)
		{
			output << currAvailableBlock;
		}
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
std::ifstream& BuddySystem::read(std::ifstream& input)
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

	// Delete the vector, and read it's length from the input file
	mAvailable.clear();
	input.read(reinterpret_cast<char*>(&lengthMAvailable), sizeof(lengthMAvailable));
	mAvailable.resize(lengthMAvailable);

	// Parse the vector
	for (auto& itMemoryAvailable : mAvailable)
	{
		// Read length of the set
		input.read(reinterpret_cast<char*>(&lengthMAvailable), sizeof(lengthMAvailable));
		if (lengthMAvailable > 0)
		{
			// Insert into the set the elements read from the file
			for (int it = 0; it < (int)lengthMAvailable; it++)
			{
				input >> poolElement;
				aux.insert(poolElement);
			}
			itMemoryAvailable = aux;
			aux.clear();
		}
	}

	// Delete the set and read it's length
	mAllocated.clear();
	input.read(reinterpret_cast<char*>(&lengthMAllocated), sizeof(lengthMAllocated));
	// Insert into the set the elements from the file
	for (int it = 0; it < (int)lengthMAllocated; it++)
	{
		input >> poolElement;
		mAllocated.insert(poolElement);
	}

	return input;
}


/*
	Function used to allocate memory
	Returns an address to an open block of memory of size:
	first number that is a power of 2 and is bigger or equal with the aSize
*/
void* __cdecl BuddySystem::allocMemory(size_t aSize, int /*aBlockUse*/, char const* /*aFileName*/, int /*aLineNumber*/)
{
	// Position will be used to find an open block of memory 
	int position = (int)std::ceil(log2(aSize));

	// We parse memory available and search for a block of memory that is 
	// bigger or equal to aSize
	while ((position < (int)mAvailable.size()) && (mAvailable[position].empty()))
	{
		position++;
	}

	// Throwing exception in case we can't allocate the memory because different reasons (see function)
	if (checkBadAlloc(aSize, position))
	{
		std::bad_alloc exception;
		throw exception;
	}

	PoolElement availableBlock = getAvailableBlock(aSize, position);

	// Update logger
	log.increaseAllocOrDealloc(-(int)availableBlock.size);

	availableBlock.size = aSize;
	mAllocated.insert(availableBlock);

	return availableBlock.address;
}


void __cdecl BuddySystem::freeMemory(void* aBlock, int /*aBlockUse*/)
{
	// Search the address that the user wants to delete in mAllocated
	auto it = mAllocated.find(PoolElement(static_cast<char*>(aBlock), 0));

	if (checkInvalidAddress(aBlock, it))
	{
		std::abort();
	}

	int deallocatedSize = (int)pow(2, (int)std::ceil(log2((*it).size)));
	PoolElement deallocatedMemory = *it;

	// Remove the adress from the allocated block
	mAllocated.erase(it);

	// Insert the address into the available memory (mAvailable)
	deallocatedMemory.size = deallocatedSize;
	insertIntoAvailableMemory(deallocatedMemory);

	// Update logger
	log.increaseAllocOrDealloc((int)deallocatedMemory.size);
}


/*
	Method used to get the memory allocated and memory requested by the user at this point
	This variables are used to evaluate the fragmentation state of the custom allocator
*/
std::pair<int, int> BuddySystem::getCurrentState() const
{
	int memoryAllocated = 0, memoryAvailable = 0, memoryRequested = 0;

	for (const auto& itMemoryAllocated : mAllocated)
	{
		memoryRequested += (int)itMemoryAllocated.size;
	}

	for (const auto& itMemoryAvailable : mAvailable)
	{
		for (const auto& currAvailableBlock : itMemoryAvailable)
		{
			memoryAvailable += (int)currAvailableBlock.size;
		}
	}
	
	memoryAllocated = (int)poolSize - memoryAvailable;

	return std::make_pair(memoryAllocated, memoryRequested);
}


/*
	Methods used to store into a file the state of the memory
	The file will be imported into excel and make a chart with the information present
*/
void BuddySystem::showCurrentState() const
{
	std::ofstream output("memoryState.txt", std::ofstream::out);
	std::list<PoolElement> auxMAvailable, auxMAllocated;

	for (const auto& currBlockAllocated : mAllocated)
	{
		auxMAllocated.push_back(PoolElement(currBlockAllocated.address, (int)pow(2, (int)std::ceil(log2(currBlockAllocated.size)))));
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

	for (const auto& currAvailableBlocks : mAvailable)
	{
		for (const auto& availableBlock : currAvailableBlocks)
		{
			auxMAvailable.push_back(availableBlock);
		}
	}

	// Sorting the list of memory available by address
	auxMAvailable.sort();

	// Merge adjacent blocks of memory available
	for (auto currAvailableBlock = auxMAvailable.begin(); std::next(currAvailableBlock) != auxMAvailable.end();)
	{
		if (currAvailableBlock->address + currAvailableBlock->size == std::next(currAvailableBlock)->address)
		{
			std::next(currAvailableBlock)->updateElement(currAvailableBlock->address, currAvailableBlock->size + std::next(currAvailableBlock)->size);
			currAvailableBlock = auxMAllocated.erase(currAvailableBlock);
		}
		else
		{
			currAvailableBlock++;
		}
	}

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
bool BuddySystem::checkBadAlloc(size_t aSize, int& position)
{
	// If we don't have enough memory available or 
	// The biggest contiguous memory is smaller than the memory requested
	if (position >= (int)mAvailable.size())
	{
		// Find the biggest continuous memory available
		position = (int)std::ceil(log2(aSize));
		if (position >= (int)mAvailable.size())
		{
			position = (int)mAvailable.size() - 1;
		}
		while ((position >= 0) && (mAvailable[position].empty()))
		{
			position--;
		}

		// Update log
		log.updateErrorLog(0, aSize, (int)pow(2, position), "Bad alloc");
		log.~Logger();

		return true;
	}

	return false;
}


/*
	This function is used to check if the user tried to deallocate a block of memory which is not allocated
*/
bool BuddySystem::checkInvalidAddress(void* aBlock, const std::set<PoolElement>::iterator& it)
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
void BuddySystem::checkMemoryLeaks()
{
	if (!mAllocated.empty())
	{
		// Update log
		log.updateWarningLog();
	}
}


/*
	Method used to find the open block that is of size:
	first number that is a power of 2 and is bigger or equal with the aSize
*/
PoolElement BuddySystem::getAvailableBlock(size_t aSize, int position)
{
	PoolElement firstNewElement, secondNewElement, currBlock;

	// Get the open block from the memory available
	currBlock = *mAvailable[position].begin();
	mAvailable[position].erase(mAvailable[position].begin());

	// While half of current open block is bigger than aSize then 
	// we split the current open block into two blocks
	position--;
	while (pow(2, position) >= aSize)
	{
		// Split the current open block into two blocks
		firstNewElement.updateElement(currBlock.address, currBlock.size / 2);
		secondNewElement.updateElement(currBlock.address + currBlock.size / 2, currBlock.size / 2);

		// The first half becomes the current block
		currBlock = firstNewElement;
		// The second half is inserted into the vector used to keep track of the available memory
		mAvailable[position].insert(secondNewElement);
		position--;
	}

	return currBlock;
}


/*
	Insert the address that was removed from the allocated block into the available memory (mAvailable)
*/
void BuddySystem::insertIntoAvailableMemory(PoolElement& deallocatedMemory)
{
	PoolElement adjacentBlock;
	int position = (int)std::ceil(log2(deallocatedMemory.size));

	// Find the adjacent block of the deallocated block 
	findAdjacentBlock(adjacentBlock, deallocatedMemory);

	// Check if the adjacent block is unallocated and available
	auto it = mAvailable[position].find(adjacentBlock);

	while (it != mAvailable[position].end())
	{
		// Merge deallocated block of memory with the adjacent block
		if ((*it).address < deallocatedMemory.address)
		{
			deallocatedMemory.address = (*it).address;
		}
		deallocatedMemory.size *= 2;

		mAvailable[position++].erase(it);

		// Find the new adjacent block of the two merged blocks
		findAdjacentBlock(adjacentBlock, deallocatedMemory);
		it = mAvailable[position].find(adjacentBlock);
	}

	mAvailable[position].insert(deallocatedMemory);
}


/*
	Method used to find the adjacent block of the deallocated block of memory
*/
void BuddySystem::findAdjacentBlock(PoolElement& buddy, const PoolElement& deallocatedMemory) const
{
	buddy.size = deallocatedMemory.size;
	if (((deallocatedMemory.address - startAddress) / deallocatedMemory.size) % 2)
	{
		buddy.address = deallocatedMemory.address - deallocatedMemory.size;
	}
	else
	{
		buddy.address = deallocatedMemory.address + deallocatedMemory.size;
	}
}


/*
	Destructor used to deallocated the memory pool, allocated at the start of the program
	Also we check if the program has memory leaks
*/
BuddySystem::~BuddySystem()
{
	checkMemoryLeaks();

	delete[] startAddress;
}